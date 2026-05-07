#include "hw_main.h"
#include "../WinRott.h"

// Set this to true to enable the hardware renderer by default.
bool iG_HardwareRenderer = true;

static SDL_GPUDevice *gpu_device = NULL;
static SDL_Renderer *gpu_renderer = NULL;
static SDL_Texture *gpu_texture = NULL;

static void HW_ResetState(void)
{
    gpu_texture = NULL;
    gpu_renderer = NULL;
    gpu_device = NULL;
}

bool HW_InitDevice(SDL_Window *window)
{
    if (!iG_HardwareRenderer) {
        return false;
    }

    gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
    if (!gpu_device) {
        SDL_Log("Failed to create SDL GPU device: %s", SDL_GetError());
        iG_HardwareRenderer = false;
        return false;
    }

    gpu_renderer = SDL_CreateGPURenderer(gpu_device, window);
    if (!gpu_renderer) {
        SDL_Log("Failed to create SDL GPU renderer: %s", SDL_GetError());
        HW_QuitDevice();
        iG_HardwareRenderer = false;
        return false;
    }

    SDL_SetRenderLogicalPresentation(
        gpu_renderer,
        iGLOBAL_SCREENWIDTH,
        iGLOBAL_SCREENHEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}

void HW_QuitDevice(void)
{
    if (gpu_texture) {
        SDL_DestroyTexture(gpu_texture);
        gpu_texture = NULL;
    }
    if (gpu_renderer) {
        SDL_DestroyRenderer(gpu_renderer);
        gpu_renderer = NULL;
    }
    if (gpu_device) {
        SDL_DestroyGPUDevice(gpu_device);
        gpu_device = NULL;
    }

    HW_ResetState();
    iG_HardwareRenderer = false;
}

void HW_UpdateScreen(SDL_Surface *surface32)
{
    static int last_w = 0;
    static int last_h = 0;

    if (!iG_HardwareRenderer || !gpu_renderer || !surface32) {
        return;
    }

    if (!gpu_texture || last_w != surface32->w || last_h != surface32->h) {
        if (gpu_texture) {
            SDL_DestroyTexture(gpu_texture);
            gpu_texture = NULL;
        }

        gpu_texture = SDL_CreateTexture(
            gpu_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            surface32->w,
            surface32->h);
        if (!gpu_texture) {
            SDL_Log("Failed to create hardware render texture: %s", SDL_GetError());
            return;
        }

        last_w = surface32->w;
        last_h = surface32->h;
    }

    if (!SDL_UpdateTexture(gpu_texture, NULL, surface32->pixels, surface32->pitch)) {
        SDL_Log("Failed to update hardware render texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderClear(gpu_renderer);
    SDL_RenderTexture(gpu_renderer, gpu_texture, NULL, NULL);
    SDL_RenderPresent(gpu_renderer);
}
