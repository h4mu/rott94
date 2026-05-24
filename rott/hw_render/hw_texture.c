#include "hw_texture.h"
#include "../rt_def.h"
#include "../z_zone.h"
#include "../byteordr.h"
#include "../lumpy.h"
#include <stdlib.h>
#include <string.h>

static SDL_GPUDevice* main_gpu_device = NULL;
static byte hw_palette[768];

#define MAX_LUMPS 16384
static SDL_GPUTexture* gpu_texture_cache[MAX_LUMPS] = {0};

extern byte* skydata[];
static SDL_GPUTexture* sky_texture = NULL;

void HW_SetPalette(const byte* new_palette)
{
    memcpy(hw_palette, new_palette, 768);
}

const byte* HW_GetPalette(void)
{
    return hw_palette;
}

static uint32_t GetRGBA(byte index)
{
    if (index == 255) return 0;
    byte r = hw_palette[index * 3 + 0];
    byte g = hw_palette[index * 3 + 1];
    byte b = hw_palette[index * 3 + 2];
    return (255 << 24) | (b << 16) | (g << 8) | r;
}

void HW_InitTextureSystem(SDL_GPUDevice* device)
{
    main_gpu_device = device;
}

static SDL_GPUTexture* CreateGPUTexture(int w, int h, const uint32_t* pixels)
{
    if (!main_gpu_device) return NULL;

    SDL_GPUTextureCreateInfo createinfo = {0};
    createinfo.type = SDL_GPU_TEXTURETYPE_2D;
    createinfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    createinfo.width = w;
    createinfo.height = h;
    createinfo.layer_count_or_depth = 1;
    createinfo.num_levels = 1;
    createinfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    SDL_GPUTexture* tex = SDL_CreateGPUTexture(main_gpu_device, &createinfo);

    SDL_GPUTransferBufferCreateInfo tb_info = {0};
    tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tb_info.size = w * h * 4;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(main_gpu_device, &tb_info);
    void* map = SDL_MapGPUTransferBuffer(main_gpu_device, tb, false);
    memcpy(map, pixels, tb_info.size);
    SDL_UnmapGPUTransferBuffer(main_gpu_device, tb);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(main_gpu_device);
    SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);
    SDL_GPUTextureTransferInfo src = { .transfer_buffer = tb, .offset = 0 };
    SDL_GPUTextureRegion dst = { .texture = tex, .w = w, .h = h, .d = 1 };
    SDL_UploadToGPUTexture(copy, &src, &dst, false);
    SDL_EndGPUCopyPass(copy);
    SDL_SubmitGPUCommandBuffer(cmd);

    SDL_ReleaseGPUTransferBuffer(main_gpu_device, tb);
    return tex;
}

hw_texture_t* HW_CreateTextureFromPatch(const patch_t* patch)
{
    hw_texture_t* tex = (hw_texture_t*)SDL_malloc(sizeof(hw_texture_t));
    tex->width = patch->width;
    tex->height = patch->height;
    tex->pixels = (uint32_t*)SDL_calloc(tex->width * tex->height, 4);

    for (int x = 0; x < patch->width; x++) {
        const byte* column = ((const byte*)patch) + patch->collumnofs[x];
        while (*column != 255) {
            byte top = *column++;
            byte len = *column++;
            for (int y = 0; y < len; y++) {
                if (top + y < tex->height) {
                    tex->pixels[(top + y) * tex->width + x] = GetRGBA(column[y]);
                }
            }
            column += len;
        }
    }
    return tex;
}

hw_texture_t* HW_CreateTextureFromTransPatch(const transpatch_t* patch)
{
    hw_texture_t* tex = (hw_texture_t*)SDL_malloc(sizeof(hw_texture_t));
    tex->width = patch->width;
    tex->height = patch->height;
    tex->pixels = (uint32_t*)SDL_calloc(tex->width * tex->height, 4);

    for (int x = 0; x < patch->width; x++) {
        const byte* column = ((const byte*)patch) + patch->collumnofs[x];
        while (*column != 255) {
            byte top = *column++;
            byte len = *column++;
            for (int y = 0; y < len; y++) {
                if (top + y < tex->height) {
                    uint32_t rgba = GetRGBA(column[y]);
                    // If transpatch, we might want to apply the translevel to alpha
                    // But ROTT uses a separate table for transparency in software.
                    // For hardware, we just use the patch as-is and let the shader handle alpha if needed.
                    // Or we can apply translevel here if it's a fixed value.
                    tex->pixels[(top + y) * tex->width + x] = rgba;
                }
            }
            column += len;
        }
    }
    return tex;
}

SDL_GPUTexture* HW_GetCachedTexture(int lump)
{
    if (lump < 0 || lump >= MAX_LUMPS) return NULL;
    if (gpu_texture_cache[lump]) return gpu_texture_cache[lump];

    int size = W_LumpLength(lump);
    byte* data = W_CacheLumpNum(lump, PU_CACHE, CvtNull, 1);
    
    int w, h;
    uint32_t* rgba = NULL;
    SDL_GPUTexture* tex = NULL;

    // Detection logic:
    // ROTT raw textures are 4096 bytes (64x64).
    // ROTT patches have a header and are usually smaller or larger but have collumnofs.
    if (size == 4096) {
        w = 64; h = 64;
        rgba = (uint32_t*)SDL_malloc(w * h * 4);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                byte index = data[x * h + y];
                rgba[y * w + x] = GetRGBA(index);
            }
        }
    } else {
        // Try to decode as patch
        patch_t* p = (patch_t*)data;
        if (p->width > 0 && p->width <= 320 && p->height > 0 && p->height <= 200) {
            hw_texture_t* hwt = HW_CreateTextureFromPatch(p);
            w = hwt->width;
            h = hwt->height;
            rgba = hwt->pixels;
            SDL_free(hwt);
        } else {
            // Fallback for unknown lumps
            w = 1; h = 1;
            rgba = (uint32_t*)SDL_calloc(1, 4);
        }
    }
    
    gpu_texture_cache[lump] = CreateGPUTexture(w, h, rgba);
    SDL_free(rgba);
    
    return gpu_texture_cache[lump];
}

SDL_GPUTexture* HW_GetSkyTexture(void)
{
    return sky_texture;
}

void HW_UpdateSkyTexture(void)
{
    if (!skydata[0] || !main_gpu_device) return;

    int width = 256;
    int height = 400;
    uint32_t* rgba = (uint32_t*)SDL_malloc(width * height * 4);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            byte index = skydata[0][x * height + y];
            rgba[y * width + x] = GetRGBA(index);
        }
    }

    if (sky_texture) SDL_ReleaseGPUTexture(main_gpu_device, sky_texture);
    sky_texture = CreateGPUTexture(width, height, rgba);
    SDL_free(rgba);
}
void HW_FreeTexture(hw_texture_t* tex)
{
    if (tex) {
        if (tex->pixels) SDL_free(tex->pixels);
        SDL_free(tex);
    }
}
