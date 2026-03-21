#include "ui_state.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <SDL3_image/SDL_image.h>

#include "image_processor.h"

bool init_ui_state(ApplicationState *state, const char *image_file_path)
{
    memset(state, 0, sizeof(ApplicationState));
    state->is_application_running = true;
    state->is_image_processing_completed = false;
    state->is_histogram_equalized = false;

    SDL_Surface *raw_image_surface = IMG_Load(image_file_path);
    if (raw_image_surface == NULL)
    {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return false;
    }

    state->image_surface = SDL_ConvertSurface(raw_image_surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(raw_image_surface);

    if (state->image_surface == NULL)
    {
        return false;
    }

    if (!is_grayscale(state->image_surface))
    {
        convert_to_grayscale(state->image_surface);
    }

    state->original_grayscale_surface = SDL_DuplicateSurface(state->image_surface);
    if (state->original_grayscale_surface == NULL)
    {
        SDL_DestroySurface(state->image_surface);
        state->image_surface = NULL;
        return false;
    }

    calculate_histogram(state->image_surface, state->histogram_frequencies);
    analyse_histogram(state);

    return true;
}

void analyse_histogram(ApplicationState *state)
{
    long long total_pixels = 0;
    long long weighted_sum = 0;

    for (int i = 0; i < 256; i++)
    {
        total_pixels += state->histogram_frequencies[i];
        weighted_sum += (long long)i * state->histogram_frequencies[i];
    }

    if (total_pixels == 0)
    {
        state->histogram_mean = 0.0;
        state->histogram_stddev = 0.0;
        state->brightness_class = BRIGHTNESS_MEDIUM;
        state->contrast_class = CONTRAST_LOW;
        return;
    }

    state->histogram_mean = (double)weighted_sum / (double)total_pixels;

    double variance_sum = 0.0;
    for (int i = 0; i < 256; i++)
    {
        double diff = (double)i - state->histogram_mean;
        variance_sum += diff * diff * state->histogram_frequencies[i];
    }
    state->histogram_stddev = sqrt(variance_sum / (double)total_pixels);

    // brightness
    if (state->histogram_mean < 85.0)
    {
        state->brightness_class = BRIGHTNESS_DARK;
    }
    else if (state->histogram_mean < 170.0)
    {
        state->brightness_class = BRIGHTNESS_MEDIUM;
    }
    else
    {
        state->brightness_class = BRIGHTNESS_BRIGHT;
    }

    // contrast
    if (state->histogram_stddev < 40.0)
    {
        state->contrast_class = CONTRAST_LOW;
    }
    else if (state->histogram_stddev < 80.0)
    {
        state->contrast_class = CONTRAST_MEDIUM;
    }
    else
    {
        state->contrast_class = CONTRAST_HIGH;
    }
}

void cleanup_ui_state(ApplicationState *state)
{
    if (state->image_surface != NULL)
    {
        SDL_DestroySurface(state->image_surface);
        state->image_surface = NULL;
    }

    if (state->original_grayscale_surface != NULL)
    {
        SDL_DestroySurface(state->original_grayscale_surface);
        state->original_grayscale_surface = NULL;
    }
}