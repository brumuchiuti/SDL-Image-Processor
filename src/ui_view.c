#include "ui_view.h"
#include <string.h>

bool init_ui_view(ApplicationView *view, ApplicationState *state)
{
    memset(view, 0, sizeof(ApplicationView));

    int image_width = state->image_surface->w;
    int image_height = state->image_surface->h;

    // Main Window Setup
    view->primary_window = SDL_CreateWindow("Imagem Principal", image_width, image_height, 0);
    view->primary_renderer = SDL_CreateRenderer(view->primary_window, NULL);
    view->primary_image_texture = SDL_CreateTextureFromSurface(view->primary_renderer, state->image_surface);

    SDL_SetWindowPosition(view->primary_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Secondary Window Setup
    int secondary_window_width = 400;
    int secondary_window_height = 450;
    view->secondary_window = SDL_CreateWindow("Painel de Controle", secondary_window_width, secondary_window_height, 0);
    view->secondary_renderer = SDL_CreateRenderer(view->secondary_window, NULL);

    int primary_window_coordinate_x;
    int primary_window_coordinate_y;
    SDL_GetWindowPosition(view->primary_window, &primary_window_coordinate_x, &primary_window_coordinate_y);
    SDL_SetWindowPosition(view->secondary_window, primary_window_coordinate_x + image_width, primary_window_coordinate_y);

    // Components Setup
    view->process_action_button.boundary_rectangle = (SDL_FRect){100.0f, 350.0f, 200.0f, 60.0f};
    view->process_action_button.normal_color = (SDL_Color){0, 120, 215, 255};
    view->process_action_button.hover_color = (SDL_Color){0, 150, 255, 255};
    view->process_action_button.is_currently_hovered = false;

    view->image_histogram_panel.boundary_rectangle = (SDL_FRect){20.0f, 20.0f, 360.0f, 300.0f};
    view->image_histogram_panel.background_color = (SDL_Color){30, 30, 30, 255};
    view->image_histogram_panel.bar_color = (SDL_Color){100, 200, 100, 255};

    return true;
}

void render_ui_view(ApplicationView *view, ApplicationState *state)
{
    // Main Window Rendering
    SDL_SetRenderDrawColor(view->primary_renderer, 0, 0, 0, 255);
    SDL_RenderClear(view->primary_renderer);
    SDL_RenderTexture(view->primary_renderer, view->primary_image_texture, NULL, NULL);
    SDL_RenderPresent(view->primary_renderer);

    // Secondary Window Rendering
    SDL_SetRenderDrawColor(view->secondary_renderer, 45, 45, 48, 255);
    SDL_RenderClear(view->secondary_renderer);

    // Histogram Panel Rendering
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           view->image_histogram_panel.background_color.r,
                           view->image_histogram_panel.background_color.g,
                           view->image_histogram_panel.background_color.b,
                           255);
    SDL_RenderFillRect(view->secondary_renderer, &view->image_histogram_panel.boundary_rectangle);

    int maximum_frequency_value = 0;
    for (int index = 0; index < 256; index++)
    {
        if (state->histogram_frequencies[index] > maximum_frequency_value)
        {
            maximum_frequency_value = state->histogram_frequencies[index];
        }
    }

    if (maximum_frequency_value > 0)
    {
        float individual_bar_width = view->image_histogram_panel.boundary_rectangle.w / 256.0f;
        SDL_SetRenderDrawColor(view->secondary_renderer,
                               view->image_histogram_panel.bar_color.r,
                               view->image_histogram_panel.bar_color.g,
                               view->image_histogram_panel.bar_color.b,
                               255);

        for (int index = 0; index < 256; index++)
        {
            float individual_bar_height = ((float)state->histogram_frequencies[index] / maximum_frequency_value) * view->image_histogram_panel.boundary_rectangle.h;
            SDL_FRect frequency_bar = {
                view->image_histogram_panel.boundary_rectangle.x + (index * individual_bar_width),
                view->image_histogram_panel.boundary_rectangle.y + view->image_histogram_panel.boundary_rectangle.h - individual_bar_height,
                individual_bar_width,
                individual_bar_height};
            SDL_RenderFillRect(view->secondary_renderer, &frequency_bar);
        }
    }

    // Button Rendering
    SDL_Color current_button_color = view->process_action_button.is_currently_hovered
                                         ? view->process_action_button.hover_color
                                         : view->process_action_button.normal_color;
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           current_button_color.r,
                           current_button_color.g,
                           current_button_color.b,
                           current_button_color.a);
    SDL_RenderFillRect(view->secondary_renderer, &view->process_action_button.boundary_rectangle);

    SDL_RenderPresent(view->secondary_renderer);
}

void cleanup_ui_view(ApplicationView *view)
{
    if (view->primary_image_texture != NULL)
        SDL_DestroyTexture(view->primary_image_texture);
    if (view->primary_renderer != NULL)
        SDL_DestroyRenderer(view->primary_renderer);
    if (view->primary_window != NULL)
        SDL_DestroyWindow(view->primary_window);
    if (view->secondary_renderer != NULL)
        SDL_DestroyRenderer(view->secondary_renderer);
    if (view->secondary_window != NULL)
        SDL_DestroyWindow(view->secondary_window);
}