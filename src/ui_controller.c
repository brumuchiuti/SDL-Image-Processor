#include "ui_controller.h"

#include <stdio.h>

#include "image_processor.h"

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