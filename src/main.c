#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "image_loader.h"
#include "image_processor.h"
#include "ui_components.h"
#include "ui_controller.h"
#include "ui_state.h"
#include "ui_view.h"
#include "utils.h"

// Main entry point for SDL Image Processor application
// Handles argument validation, SDL initialization, and main event loop
int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Falha ao inicializar a biblioteca SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    ApplicationState application_state;
    ApplicationView application_view;

    if (!init_ui_state(&application_state, filename)) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (!init_ui_view(&application_view, &application_state)) {
        cleanup_ui_state(&application_state);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    while (application_state.is_application_running) {
        process_user_interactions(&application_state, &application_view);
        render_ui_view(&application_view, &application_state);
        SDL_Delay(from_fps_to_delay_in_ms(60));
    }

    cleanup_ui_view(&application_view);
    cleanup_ui_state(&application_state);

    SDL_Quit();

    return EXIT_SUCCESS;
}
