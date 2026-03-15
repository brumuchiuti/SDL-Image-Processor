#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

const char* ALLOWED_EXTENSIONS[] = {
    "png",
    "jpg",
    "bmp"
};
const int NUM_ALLOWED_EXTENSIONS = sizeof(ALLOWED_EXTENSIONS) / sizeof(ALLOWED_EXTENSIONS[0]);

bool has_allowed_extension(const char* filename) {
    const char* extension = strrchr(filename, '.');
    if (!extension || extension == filename) {
        return false;
    }

    extension++; // ignore the dot

    for (int i = 0; i < NUM_ALLOWED_EXTENSIONS; i++) {
        if (strcmp(extension, ALLOWED_EXTENSIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}

int load_image(SDL_Surface* surface, const char *filename) {
    if (!has_allowed_extension(filename)) {
        fprintf(stderr, "Unsupported file format: %s\n", filename);
        return EXIT_FAILURE;
    }

    SDL_Surface* raw_surface = IMG_Load(filename);

    if (raw_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Convert the surface to a known pixel format: RGBA32 (32 bits per pixel, with alpha channel)
    surface = SDL_ConvertSurface(raw_surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(raw_surface); 

    if (!surface) {
        fprintf(stderr, "Failed to convert surface: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Lock the surface to access the pixel data
    SDL_LockSurface(surface);

    return EXIT_SUCCESS;
}

int unload_image(SDL_Surface* surface) {
    if (surface) {
        SDL_UnlockSurface(surface);
        SDL_DestroySurface(surface);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    
    SDL_Surface* surface = NULL;
    load_image(surface, filename);
    unload_image(surface);

    return EXIT_SUCCESS;
}
