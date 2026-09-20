#ifndef HW_TEXTURE_H
#define HW_TEXTURE_H

#include "../rt_def.h"
#include "../lumpy.h"
#include <stdint.h>

typedef struct {
    int width;
    int height;
    uint32_t *pixels; // RGBA 8888, stored row-major: pixels[y * width + x]
} hw_texture_t;

// The global palette, should be set whenever the 8-bit palette changes.
// Expects an array of 768 bytes (RGB triplets, 0-255).
void HW_SetPalette(const byte* new_palette);

// Extract the current palette.
const byte* HW_GetPalette(void);

// Convert a column-major raw wall patch (e.g. 64x64)
hw_texture_t* HW_CreateTextureFromRaw(const byte* raw_data, int width, int height);

// Convert a patch_t (used for sprites and some walls)
hw_texture_t* HW_CreateTextureFromPatch(const patch_t* patch);

// Convert a transpatch_t (used for transparent sprites/effects)
hw_texture_t* HW_CreateTextureFromTransPatch(const transpatch_t* patch);

// Convert a pic_t (used for UI elements)
hw_texture_t* HW_CreateTextureFromPic(const pic_t* pic);

#include <SDL3/SDL.h>

// Cache management
SDL_GPUTexture* HW_GetCachedTexture(int lump);
SDL_GPUTexture* HW_GetSkyTexture(void);
void HW_UpdateSkyTexture(void);
void HW_InitTextureSystem(SDL_GPUDevice* device);

// Free an allocated texture
void HW_FreeTexture(hw_texture_t* tex);

#endif
