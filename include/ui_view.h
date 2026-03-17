#ifndef UI_VIEW_H
#define UI_VIEW_H

#include <SDL3_ttf/SDL_ttf.h>

#include "ui_state.h"
#include "ui_components.h"

typedef struct {
    SDL_Window* primary_window;
    SDL_Renderer* primary_renderer;
    SDL_Texture* primary_image_texture;
    
    SDL_Window* secondary_window;
    SDL_Renderer* secondary_renderer;

    TTF_Font* ui_font;
    
    ButtonComponent process_action_button;
    HistogramComponent image_histogram_panel;
} ApplicationView;

bool init_ui_view(ApplicationView* view, ApplicationState* state);

void render_ui_view(ApplicationView* view, ApplicationState* state);

void cleanup_ui_view(ApplicationView* view);

#endif // UI_VIEW_H