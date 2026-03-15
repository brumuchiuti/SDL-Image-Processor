#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

/**
 * Loads an image from the specified file and converts it to a known pixel format (RGBA32).
 * @param filename The path to the image file to load.
 * @return A pointer to the loaded SDL_Surface, or NULL if loading failed.
 */
SDL_Surface* load_image(const char *filename);

/**
 * Unloads the specified image surface and frees the associated memory.
 * @param surface The SDL_Surface to unload.
 * @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int unload_image(SDL_Surface* surface);

#endif // IMAGE_LOADER_H