#include "ui_controller.h"

#include <stdio.h>

#include <SDL3_image/SDL_image.h>

#include "image_processor.h"

// Output file path for saving processed images
#define OUTPUT_IMAGE_PATH "output_image.png"

/**
 * save_current_image_to_png - Saves the current image surface to a PNG file.
 * @state: Pointer to the application state containing the image surface
 *
 * Returns: true if save is successful, false otherwise
 *
 * This function exports the currently displayed image (whether original or
 * equalized) to the OUTPUT_IMAGE_PATH. If the file already exists, it will
 * be overwritten. Errors are logged to stderr.
 */
static bool save_current_image_to_png(const ApplicationState *state)
{
    if (!state || !state->image_surface)
    {
        fprintf(stderr, "[SAVE] No image surface available to save.\n");
        return false;
    }

    if (!IMG_SavePNG(state->image_surface, OUTPUT_IMAGE_PATH))
    {
        fprintf(stderr, "[SAVE] Failed to save %s: %s\n", OUTPUT_IMAGE_PATH, SDL_GetError());
        return false;
    }

    printf("[SAVE] Image saved to %s\n", OUTPUT_IMAGE_PATH);
    return true;
}

static void refresh_processed_view(ApplicationState *state, ApplicationView *view)
{
    calculate_histogram(state->image_surface, state->histogram_frequencies);
    analyse_histogram(state);
    if (!update_primary_image_texture(view, state))
    {
        fprintf(stderr, "Warning: failed to refresh primary texture after processing.\n");
    }
}

static void toggle_histogram_equalization(ApplicationState *state, ApplicationView *view)
{
    if (!state->image_surface || !state->original_grayscale_surface)
        return;

    if (!state->is_histogram_equalized)
    {
        SDL_BlitSurface(state->original_grayscale_surface, NULL, state->image_surface, NULL);
        equalize_histogram(state->image_surface);
        state->is_histogram_equalized = true;
    }
    else
    {
        SDL_BlitSurface(state->original_grayscale_surface, NULL, state->image_surface, NULL);
        state->is_histogram_equalized = false;
    }

    state->is_image_processing_completed = state->is_histogram_equalized;
    refresh_processed_view(state, view);
}

void process_user_interactions(ApplicationState *state, ApplicationView *view)
{
    SDL_Event user_event;

    while (SDL_PollEvent(&user_event))
    {
        if (user_event.type == SDL_EVENT_QUIT)
        {
            state->is_application_running = false;
            continue;
        }

        if (user_event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            state->is_application_running = false;
            continue;
        }

        if (user_event.type == SDL_EVENT_KEY_DOWN)
        {
            if (!user_event.key.repeat && user_event.key.scancode == SDL_SCANCODE_S)
            {
                save_current_image_to_png(state);
            }
            continue;
        }

        if (user_event.type == SDL_EVENT_MOUSE_MOTION &&
            user_event.motion.windowID == SDL_GetWindowID(view->secondary_window))
        {
            view->process_action_button.is_currently_hovered = is_point_inside_rectangle(
                user_event.motion.x,
                user_event.motion.y,
                view->process_action_button.boundary_rectangle);
            continue;
        }

        if (user_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
            user_event.button.button == SDL_BUTTON_LEFT &&
            user_event.button.windowID == SDL_GetWindowID(view->secondary_window))
        {
            bool is_over_button = is_point_inside_rectangle(
                user_event.button.x,
                user_event.button.y,
                view->process_action_button.boundary_rectangle);

            view->process_action_button.is_currently_hovered = is_over_button;
            view->process_action_button.is_currently_clicked = is_over_button;
            continue;
        }

        if (user_event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            user_event.button.button == SDL_BUTTON_LEFT &&
            user_event.button.windowID == SDL_GetWindowID(view->secondary_window))
        {
            bool is_over_button = is_point_inside_rectangle(
                user_event.button.x,
                user_event.button.y,
                view->process_action_button.boundary_rectangle);

            if (view->process_action_button.is_currently_clicked && is_over_button)
            {
                printf("[EVENT] Process Action Button Clicked!\n");
                toggle_histogram_equalization(state, view);
            }

            view->process_action_button.is_currently_clicked = false;
            view->process_action_button.is_currently_hovered = is_over_button;
        }
    }
}