#include "ui_state.h"

#include <stdio.h>
#include <string.h>

#include <SDL3_image/SDL_image.h>

#include "image_processor.h"

bool init_ui_state(ApplicationState* state, const char* image_file_path) {
    memset(state, 0, sizeof(ApplicationState));
    state->is_application_running = true;
    state->is_image_processing_completed = false;

    SDL_Surface* raw_image_surface = IMG_Load(image_file_path);
    if (raw_image_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return false;
    }
    
    state->image_surface = SDL_ConvertSurface(raw_image_surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(raw_image_surface);
    
    if (state->image_surface == NULL) {
        return false;
    }

    if (!is_grayscale(state->image_surface)) {
        convert_to_grayscale(state->image_surface);
    }
    
    calculate_histogram(state->image_surface, state->histogram_frequencies);

    return true;
}

void cleanup_ui_state(ApplicationState* state) {
    if (state->image_surface != NULL) {
        SDL_DestroySurface(state->image_surface);
        state->image_surface = NULL;
    }
}