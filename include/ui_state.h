#ifndef UI_STATE_H
#define UI_STATE_H

#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct {
    bool is_application_running;
    bool is_image_processing_completed;
    
    SDL_Surface* image_surface;
    int histogram_frequencies[256];
} ApplicationState;

/**
 * Initialize the application state, including loading the image and calculating its histogram.
 * @param state Pointer to the ApplicationState structure to initialize.
 * @param image_file_path Path to the image file to load.
 * @return true if initialization was successful, false otherwise.
 */
bool init_ui_state(ApplicationState* state, const char* image_file_path);

/**
 * Clean up resources associated with the application state, such as freeing surfaces.
 * @param state Pointer to the ApplicationState structure to clean up.
 */
void cleanup_ui_state(ApplicationState* state);

#endif // UI_STATE_H