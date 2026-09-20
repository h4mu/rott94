/*
 * Android builds package the game as a shared library named "main".
 * The real SDL entry point comes from rt_main.c via SDL_main.h, but CMake
 * still needs at least one source file directly attached to this wrapper.
 */

void rott_android_main_stub(void)
{
}
