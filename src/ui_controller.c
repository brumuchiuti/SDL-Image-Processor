#include "ui_controller.h"
#include <stdio.h>

void process_user_interactions(ApplicationState *state, ApplicationView *view)
{
    SDL_Event user_event;

    while (SDL_PollEvent(&user_event))
    {

        if (user_event.type == SDL_EVENT_QUIT)
        {
            state->is_application_running = false;
        }

        if (user_event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            state->is_application_running = false;
        }

        // Handle mouse events for the secondary window (where the button is located)
        if (user_event.window.windowID == SDL_GetWindowID(view->secondary_window))
        {

            float mouse_x;
            float mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            // Update the visual component state (Hover)
            view->process_action_button.is_currently_hovered = is_point_inside_rectangle(
                mouse_x,
                mouse_y,
                view->process_action_button.boundary_rectangle);

            // Register the mouse click event on the button
            if (user_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && user_event.button.button == SDL_BUTTON_LEFT)
            {
                if (view->process_action_button.is_currently_hovered)
                {
                    printf("[EVENT] Process Action Button Clicked!\n");
                    state->is_image_processing_completed = true;
                    // TODO: item 5
                }
            }
        }
    }
}