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

bool is_grayscale(SDL_Surface* surface) {
    Uint8* pixels = (Uint8*)surface->pixels;

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            int offset = (y * surface->pitch) + (x * 4);

            // Uint8 = unsigned char (8 bits) = 0-255
            Uint8 r = pixels[offset + 0];
            Uint8 g = pixels[offset + 1];
            Uint8 b = pixels[offset + 2];

            // gray if R, G, and B values are the same
            if (r != g || r != b) {
                return false; 
            }
        }
    }
    return true;
}

void convert_to_grayscale(SDL_Surface* surface) {
    Uint8* pixels = (Uint8*)surface->pixels;

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            int offset = (y * surface->pitch) + (x * 4);
            Uint8 r = pixels[offset + 0];
            Uint8 g = pixels[offset + 1];
            Uint8 b = pixels[offset + 2];

            // Formula: Y = 0.2125 * R + 0.7154 * G + 0.0721 * B
            float luminance = (0.2125f * r) + (0.7154f * g) + (0.0721f * b);
            
            Uint8 grayScaleValue = (Uint8)luminance;

            pixels[offset + 0] = grayScaleValue;
            pixels[offset + 1] = grayScaleValue;
            pixels[offset + 2] = grayScaleValue;
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    
    SDL_Surface* surface = load_image(filename);

    if (is_grayscale(surface)) {
        printf("The image is already grayscale.\n");
    } else {
        printf("The image is not grayscale. Converting...\n");
        convert_to_grayscale(surface);
        printf("Conversion complete.\n");
    }

    unload_image(surface);
    return EXIT_SUCCESS;
}
