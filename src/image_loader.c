#include "image_loader.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

SDL_Surface* load_image(const char *filename) {
    if (!has_allowed_extension(filename)) {
        fprintf(stderr, "Unsupported file format: %s\n", filename);
        return NULL;
    }

    SDL_Surface* raw_surface = IMG_Load(filename);

    if (raw_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return NULL;
    }

    // Convert the surface to a known pixel format: RGBA32 (32 bits per pixel, with alpha channel)
    SDL_Surface* surface = SDL_ConvertSurface(raw_surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(raw_surface); 

    if (!surface) {
        fprintf(stderr, "Failed to convert surface: %s\n", SDL_GetError());
        return NULL;
    }

    // Lock the surface to access the pixel data
    SDL_LockSurface(surface);

    printf("Loaded image: %s\n", filename);
    return surface;
}

int unload_image(SDL_Surface* surface) {
    if (surface) {
        SDL_UnlockSurface(surface);
        SDL_DestroySurface(surface);
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "No surface to unload.\n");
    return EXIT_FAILURE;
}