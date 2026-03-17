#ifndef UI_STATE_H
#define UI_STATE_H

#include <stdbool.h>
#include <SDL3/SDL.h>

typedef enum {
    BRIGHTNESS_DARK,
    BRIGHTNESS_MEDIUM,
    BRIGHTNESS_BRIGHT
} BrightnessClass;

typedef enum {
    CONTRAST_LOW,
    CONTRAST_MEDIUM,
    CONTRAST_HIGH
} ContrastClass;

typedef struct {
    bool is_application_running;
    bool is_image_processing_completed;
    
    SDL_Surface* image_surface;
    int histogram_frequencies[256];

    /* Histogram analysis results */
    double histogram_mean;
    double histogram_stddev;
    BrightnessClass brightness_class;
    ContrastClass contrast_class;
} ApplicationState;

/**
 * Initialize the application state, including loading the image and calculating its histogram.
 * @param state Pointer to the ApplicationState structure to initialize.
 * @param image_file_path Path to the image file to load.
 * @return true if initialization was successful, false otherwise.
 */
bool init_ui_state(ApplicationState* state, const char* image_file_path);

/**
 * Compute mean, standard deviation and classification from the histogram.
 * @param state Pointer to the ApplicationState structure (histogram_frequencies must be filled).
 */
void analyse_histogram(ApplicationState* state);

/**
 * Clean up resources associated with the application state, such as freeing surfaces.
 * @param state Pointer to the ApplicationState structure to clean up.
 */
void cleanup_ui_state(ApplicationState* state);

#endif // UI_STATE_H