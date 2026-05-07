#ifndef HW_MAIN_H
#define HW_MAIN_H

#include <stdbool.h>
#include <SDL3/SDL.h>

extern bool iG_HardwareRenderer;

// Initialize the GPU device and claim the window.
bool HW_InitDevice(SDL_Window* window);

// Cleanup the GPU device.
void HW_QuitDevice(void);

// Upload the 32-bit software buffer to a texture and render it full screen.
void HW_UpdateScreen(SDL_Surface* surface32);

#endif
