#include "ui_view.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

// render_text
static bool render_text(ApplicationView *view, const char *text,
                         float x, float y, SDL_Color color)
{
    if (!view->ui_font || !text) return false;

    SDL_Surface *text_surface = TTF_RenderText_Blended(view->ui_font, text, 0, color);
    if (!text_surface) return false;

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(view->secondary_renderer, text_surface);
    SDL_DestroySurface(text_surface);
    if (!text_texture) return false;

    float tex_w, tex_h;
    SDL_GetTextureSize(text_texture, &tex_w, &tex_h);

    SDL_FRect dst = { x, y, tex_w, tex_h };
    SDL_RenderTexture(view->secondary_renderer, text_texture, NULL, &dst);
    SDL_DestroyTexture(text_texture);
    return true;
}

// init_ui_view
 
bool init_ui_view(ApplicationView *view, ApplicationState *state)
{
    memset(view, 0, sizeof(ApplicationView));

    int image_width  = state->image_surface->w;
    int image_height = state->image_surface->h;

    // Primary Window
    view->primary_window   = SDL_CreateWindow("Imagem Principal", image_width, image_height, 0);
    view->primary_renderer = SDL_CreateRenderer(view->primary_window, NULL);
    view->primary_image_texture = SDL_CreateTextureFromSurface(view->primary_renderer, state->image_surface);
    SDL_SetWindowPosition(view->primary_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Secondary Window
    int secondary_window_width  = 400;
    int secondary_window_height = 500;
    view->secondary_window   = SDL_CreateWindow("Painel de Controle", secondary_window_width, secondary_window_height, 0);
    view->secondary_renderer = SDL_CreateRenderer(view->secondary_window, NULL);

    int primary_x, primary_y;
    SDL_GetWindowPosition(view->primary_window, &primary_x, &primary_y);
    SDL_SetWindowPosition(view->secondary_window, primary_x + image_width, primary_y);

    // SDL_ttf
    if (!TTF_Init()) {
        fprintf(stderr, "SDL_ttf init failed: %s\n", SDL_GetError());
    }

    const char *font_candidates[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "C:/Windows/Fonts/arial.ttf",
        NULL
    };
    view->ui_font = NULL;
    for (int i = 0; font_candidates[i] != NULL; i++) {
        view->ui_font = TTF_OpenFont(font_candidates[i], 14);
        if (view->ui_font) break;
    }
    if (!view->ui_font) {
        fprintf(stderr, "Warning: no font found for SDL_ttf; text labels will be skipped.\n");
    }

    // Components
    view->process_action_button.boundary_rectangle = (SDL_FRect){100.0f, 420.0f, 200.0f, 50.0f};
    view->process_action_button.normal_color       = (SDL_Color){0, 120, 215, 255};
    view->process_action_button.hover_color        = (SDL_Color){0, 150, 255, 255};
    view->process_action_button.is_currently_hovered = false;

    view->image_histogram_panel.boundary_rectangle = (SDL_FRect){20.0f, 20.0f, 360.0f, 280.0f};
    view->image_histogram_panel.background_color   = (SDL_Color){30, 30, 30, 255};
    view->image_histogram_panel.bar_color          = (SDL_Color){100, 200, 100, 255};

    return true;
}

// render_ui_view
void render_ui_view(ApplicationView *view, ApplicationState *state)
{
    // Primary Window
    SDL_SetRenderDrawColor(view->primary_renderer, 0, 0, 0, 255);
    SDL_RenderClear(view->primary_renderer);
    SDL_RenderTexture(view->primary_renderer, view->primary_image_texture, NULL, NULL);
    SDL_RenderPresent(view->primary_renderer);

    // Secondary Window
    SDL_SetRenderDrawColor(view->secondary_renderer, 45, 45, 48, 255);
    SDL_RenderClear(view->secondary_renderer);

    // histogram background
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           view->image_histogram_panel.background_color.r,
                           view->image_histogram_panel.background_color.g,
                           view->image_histogram_panel.background_color.b,
                           255);
    SDL_RenderFillRect(view->secondary_renderer, &view->image_histogram_panel.boundary_rectangle);

    // logarithmic scale
    int maximum_frequency = 0;
    for (int i = 0; i < 256; i++) {
        if (state->histogram_frequencies[i] > maximum_frequency)
            maximum_frequency = state->histogram_frequencies[i];
    }

    if (maximum_frequency > 0) {
        float bar_w   = view->image_histogram_panel.boundary_rectangle.w / 256.0f;
        float panel_h = view->image_histogram_panel.boundary_rectangle.h;
        float log_max = logf((float)maximum_frequency + 1.0f);

        SDL_SetRenderDrawColor(view->secondary_renderer,
                               view->image_histogram_panel.bar_color.r,
                               view->image_histogram_panel.bar_color.g,
                               view->image_histogram_panel.bar_color.b,
                               255);
        for (int i = 0; i < 256; i++) {
            float bar_h = (logf((float)state->histogram_frequencies[i] + 1.0f) / log_max)
                          * panel_h;
            SDL_FRect bar = {
                view->image_histogram_panel.boundary_rectangle.x + (i * bar_w),
                view->image_histogram_panel.boundary_rectangle.y + panel_h - bar_h,
                bar_w,
                bar_h
            };
            SDL_RenderFillRect(view->secondary_renderer, &bar);
        }
    }

    // labels
    if (view->ui_font) {
        SDL_Color label_color = {180, 180, 180, 255};
        float panel_bottom = view->image_histogram_panel.boundary_rectangle.y
                           + view->image_histogram_panel.boundary_rectangle.h + 4.0f;
        render_text(view, "0",
                    view->image_histogram_panel.boundary_rectangle.x,
                    panel_bottom, label_color);
        render_text(view, "255",
                    view->image_histogram_panel.boundary_rectangle.x
                        + view->image_histogram_panel.boundary_rectangle.w - 22.0f,
                    panel_bottom, label_color);
        render_text(view, "Intensidade",
                    view->image_histogram_panel.boundary_rectangle.x + 145.0f,
                    panel_bottom, label_color);
    }

    // analisys section
    if (view->ui_font) {
        char line[160];
        SDL_Color title_color = {220, 220, 220, 255};
        SDL_Color value_color = {100, 220, 100, 255};
        SDL_Color hint_color  = {140, 140, 140, 255};

        float tx     = 20.0f;
        float ty     = 320.0f;
        float line_h = 20.0f;

        render_text(view, "Analise do Histograma:", tx, ty, title_color);
        ty += line_h + 4.0f;

        const char *brightness_label;
        switch (state->brightness_class) {
            case BRIGHTNESS_DARK:   brightness_label = "escura"; break;
            case BRIGHTNESS_BRIGHT: brightness_label = "clara";  break;
            default:                brightness_label = "media";  break;
        }
        snprintf(line, sizeof(line), "Media de intensidade: %.1f  =>  Imagem %s",
                 state->histogram_mean, brightness_label);
        render_text(view, line, tx, ty, value_color);
        ty += line_h;

        const char *contrast_label;
        switch (state->contrast_class) {
            case CONTRAST_LOW:  contrast_label = "baixo"; break;
            case CONTRAST_HIGH: contrast_label = "alto";  break;
            default:            contrast_label = "medio"; break;
        }
        snprintf(line, sizeof(line), "Desvio padrao: %.1f  =>  Contraste %s",
                 state->histogram_stddev, contrast_label);
        render_text(view, line, tx, ty, value_color);
        ty += line_h + 2.0f;

        // legend
        render_text(view, "Media: <85 escura | 85-170 media | >170 clara",
                    tx, ty, hint_color);
        ty += line_h;
        render_text(view, "Desvio: <40 baixo | 40-80 medio | >80 alto",
                    tx, ty, hint_color);
    }

    // button
    SDL_Color btn_color = view->process_action_button.is_currently_hovered
                        ? view->process_action_button.hover_color
                        : view->process_action_button.normal_color;
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           btn_color.r, btn_color.g, btn_color.b, btn_color.a);
    SDL_RenderFillRect(view->secondary_renderer, &view->process_action_button.boundary_rectangle);

    if (view->ui_font) {
        SDL_Color btn_text = {255, 255, 255, 255};
        render_text(view, "Processar Imagem",
                    view->process_action_button.boundary_rectangle.x + 36.0f,
                    view->process_action_button.boundary_rectangle.y + 16.0f,
                    btn_text);
    }

    SDL_RenderPresent(view->secondary_renderer);
}

// cleanup_ui_view

void cleanup_ui_view(ApplicationView *view)
{
    if (view->ui_font) {
        TTF_CloseFont(view->ui_font);
        view->ui_font = NULL;
    }
    TTF_Quit();

    if (view->primary_image_texture) SDL_DestroyTexture(view->primary_image_texture);
    if (view->primary_renderer)      SDL_DestroyRenderer(view->primary_renderer);
    if (view->primary_window)        SDL_DestroyWindow(view->primary_window);
    if (view->secondary_renderer)    SDL_DestroyRenderer(view->secondary_renderer);
    if (view->secondary_window)      SDL_DestroyWindow(view->secondary_window);
}