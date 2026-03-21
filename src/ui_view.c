#include "ui_view.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    SDL_Color clear_color;
} PrimaryImageProps;

typedef struct
{
    SDL_Color background_color;
} SecondaryWindowProps;

typedef struct
{
    SDL_FRect panel_rect;
    SDL_Color panel_color;
    SDL_Color bar_color;
    SDL_Color label_color;
    const char *left_label;
    const char *right_label;
    const char *center_label;
    float right_label_offset_x;
    float center_label_offset_x;
} HistogramProps;

typedef struct
{
    float text_x;
    float text_y;
    float line_height;
    SDL_Color title_color;
    SDL_Color value_color;
    SDL_Color hint_color;
    const char *title;
    const char *brightness_hint;
    const char *contrast_hint;
} AnalysisSectionProps;

typedef struct
{
    SDL_FRect rect;
    SDL_Color normal_color;
    SDL_Color hover_color;
    SDL_Color clicked_color;
    bool is_clicked;
    bool is_hovered;
    SDL_Color text_color;
    const char *text;
    float text_offset_x;
    float text_offset_y;
} ActionButtonProps;

bool init_ui_view(ApplicationView *view, ApplicationState *state)
{
    memset(view, 0, sizeof(ApplicationView));

    int image_width = state->image_surface->w;
    int image_height = state->image_surface->h;

    // Primary Window
    view->primary_window = SDL_CreateWindow("Imagem Principal", image_width, image_height, 0);
    view->primary_renderer = SDL_CreateRenderer(view->primary_window, NULL);
    view->primary_image_texture = SDL_CreateTextureFromSurface(view->primary_renderer, state->image_surface);
    SDL_SetWindowPosition(view->primary_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Secondary Window
    int secondary_window_width = 400;
    int secondary_window_height = 500;
    view->secondary_window = SDL_CreateWindow("Painel de Controle", secondary_window_width, secondary_window_height, 0);
    view->secondary_renderer = SDL_CreateRenderer(view->secondary_window, NULL);

    int primary_x, primary_y;
    SDL_GetWindowPosition(view->primary_window, &primary_x, &primary_y);
    SDL_SetWindowPosition(view->secondary_window, primary_x + image_width, primary_y);

    // SDL_ttf
    if (!TTF_Init())
    {
        fprintf(stderr, "SDL_ttf init failed: %s\n", SDL_GetError());
    }

    const char *font_candidates[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/MesloLGS-NF-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "C:/Windows/Fonts/arial.ttf",
        NULL};

    view->ui_font = NULL;
    for (int i = 0; font_candidates[i] != NULL; i++)
    {
        view->ui_font = TTF_OpenFont(font_candidates[i], 14);
        if (view->ui_font)
            break;
    }
    if (!view->ui_font)
    {
        fprintf(stderr, "Warning: no font found for SDL_ttf; text labels will be skipped.\n");
    }

    view->process_action_button.boundary_rectangle = (SDL_FRect){100.0f, 420.0f, 200.0f, 50.0f};
    view->process_action_button.normal_color = (SDL_Color){0, 120, 215, 255};
    view->process_action_button.hover_color = (SDL_Color){0, 150, 255, 255};
    view->process_action_button.is_currently_hovered = false;
    view->process_action_button.is_currently_clicked = false;

    view->image_histogram_panel.boundary_rectangle = (SDL_FRect){20.0f, 20.0f, 360.0f, 280.0f};
    view->image_histogram_panel.background_color = (SDL_Color){30, 30, 30, 255};
    view->image_histogram_panel.bar_color = (SDL_Color){100, 200, 100, 255};

    return true;
}

bool update_primary_image_texture(ApplicationView *view, ApplicationState *state)
{
    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(view->primary_renderer, state->image_surface);
    if (!new_texture)
    {
        fprintf(stderr, "Failed to recreate primary texture: %s\n", SDL_GetError());
        return false;
    }

    if (view->primary_image_texture)
    {
        SDL_DestroyTexture(view->primary_image_texture);
    }
    view->primary_image_texture = new_texture;
    return true;
}

static bool render_text(ApplicationView *view, const char *text, float x, float y, SDL_Color color)
{
    if (!view->ui_font || !text)
        return false;

    SDL_Surface *text_surface = TTF_RenderText_Blended(view->ui_font, text, 0, color);
    if (!text_surface)
        return false;

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(view->secondary_renderer, text_surface);
    SDL_DestroySurface(text_surface);
    if (!text_texture)
        return false;

    float tex_w, tex_h;
    SDL_GetTextureSize(text_texture, &tex_w, &tex_h);

    SDL_FRect dst = {x, y, tex_w, tex_h};
    SDL_RenderTexture(view->secondary_renderer, text_texture, NULL, &dst);
    SDL_DestroyTexture(text_texture);
    return true;
}

static PrimaryImageProps create_primary_image_component(void)
{
    PrimaryImageProps props = {
        .clear_color = {0, 0, 0, 255}};
    return props;
}

static void render_primary_image_component(ApplicationView *view, const PrimaryImageProps *props)
{
    SDL_SetRenderDrawColor(view->primary_renderer,
                           props->clear_color.r,
                           props->clear_color.g,
                           props->clear_color.b,
                           props->clear_color.a);
    SDL_RenderClear(view->primary_renderer);
    SDL_RenderTexture(view->primary_renderer, view->primary_image_texture, NULL, NULL);
    SDL_RenderPresent(view->primary_renderer);
}

static SecondaryWindowProps create_secondary_window_component(void)
{
    SecondaryWindowProps props = {
        .background_color = {45, 45, 48, 255}};
    return props;
}

static void render_secondary_window_component(ApplicationView *view, const SecondaryWindowProps *props)
{
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           props->background_color.r,
                           props->background_color.g,
                           props->background_color.b,
                           props->background_color.a);
    SDL_RenderClear(view->secondary_renderer);
}

static HistogramProps create_histogram_component(const ApplicationView *view)
{
    HistogramProps props = {
        .panel_rect = view->image_histogram_panel.boundary_rectangle,
        .panel_color = view->image_histogram_panel.background_color,
        .bar_color = view->image_histogram_panel.bar_color,
        .label_color = {180, 180, 180, 255},
        .left_label = "0",
        .right_label = "255",
        .center_label = "Intensidade",
        .right_label_offset_x = 22.0f,
        .center_label_offset_x = 145.0f};
    return props;
}

static void render_histogram_component(ApplicationView *view, ApplicationState *state, const HistogramProps *props)
{
    SDL_SetRenderDrawColor(view->secondary_renderer,
                           props->panel_color.r,
                           props->panel_color.g,
                           props->panel_color.b,
                           props->panel_color.a);
    SDL_RenderFillRect(view->secondary_renderer, &props->panel_rect);

    int maximum_frequency = 0;
    for (int i = 0; i < 256; i++)
    {
        if (state->histogram_frequencies[i] > maximum_frequency)
            maximum_frequency = state->histogram_frequencies[i];
    }

    if (maximum_frequency > 0)
    {
        float bar_w = props->panel_rect.w / 256.0f;
        float panel_h = props->panel_rect.h;
        float log_max = logf((float)maximum_frequency + 1.0f);

        SDL_SetRenderDrawColor(view->secondary_renderer,
                               props->bar_color.r,
                               props->bar_color.g,
                               props->bar_color.b,
                               props->bar_color.a);
        for (int i = 0; i < 256; i++)
        {
            float bar_h = (logf((float)state->histogram_frequencies[i] + 1.0f) / log_max) * panel_h;
            SDL_FRect bar = {
                props->panel_rect.x + (i * bar_w),
                props->panel_rect.y + panel_h - bar_h,
                bar_w,
                bar_h};
            SDL_RenderFillRect(view->secondary_renderer, &bar);
        }
    }

    if (view->ui_font)
    {
        float panel_bottom = props->panel_rect.y + props->panel_rect.h + 4.0f;
        render_text(view, props->left_label, props->panel_rect.x, panel_bottom, props->label_color);
        render_text(view,
                    props->right_label,
                    props->panel_rect.x + props->panel_rect.w - props->right_label_offset_x,
                    panel_bottom,
                    props->label_color);
        render_text(view,
                    props->center_label,
                    props->panel_rect.x + props->center_label_offset_x,
                    panel_bottom,
                    props->label_color);
    }
}

static AnalysisSectionProps create_analysis_section_component(void)
{
    AnalysisSectionProps props = {
        .text_x = 20.0f,
        .text_y = 320.0f,
        .line_height = 20.0f,
        .title_color = {220, 220, 220, 255},
        .value_color = {100, 220, 100, 255},
        .hint_color = {140, 140, 140, 255},
        .title = "Analise do Histograma:",
        .brightness_hint = "Media: <85 escura | 85-170 media | >170 clara",
        .contrast_hint = "Desvio: <40 baixo | 40-80 medio | >80 alto"};
    return props;
}

static void render_analysis_section_component(ApplicationView *view, ApplicationState *state, const AnalysisSectionProps *props)
{
    if (!view->ui_font)
        return;

    char line[160];
    float ty = props->text_y;

    render_text(view, props->title, props->text_x, ty, props->title_color);
    ty += props->line_height + 4.0f;

    const char *brightness_label;
    switch (state->brightness_class)
    {
    case BRIGHTNESS_DARK:
        brightness_label = "escura";
        break;
    case BRIGHTNESS_BRIGHT:
        brightness_label = "clara";
        break;
    default:
        brightness_label = "media";
        break;
    }

    snprintf(line, sizeof(line), "Media de intensidade: %.1f  =>  Imagem %s",
             state->histogram_mean, brightness_label);
    render_text(view, line, props->text_x, ty, props->value_color);
    ty += props->line_height;

    const char *contrast_label;
    switch (state->contrast_class)
    {
    case CONTRAST_LOW:
        contrast_label = "baixo";
        break;
    case CONTRAST_HIGH:
        contrast_label = "alto";
        break;
    default:
        contrast_label = "medio";
        break;
    }

    snprintf(line, sizeof(line), "Desvio padrao: %.1f  =>  Contraste %s",
             state->histogram_stddev, contrast_label);
    render_text(view, line, props->text_x, ty, props->value_color);
    ty += props->line_height + 2.0f;

    render_text(view, props->brightness_hint, props->text_x, ty, props->hint_color);
    ty += props->line_height;
    render_text(view, props->contrast_hint, props->text_x, ty, props->hint_color);
}

static ActionButtonProps create_action_button_component(const ApplicationView *view, const ApplicationState *state)
{
    ActionButtonProps props = {
        .rect = view->process_action_button.boundary_rectangle,
        .normal_color = view->process_action_button.normal_color,
        .hover_color = view->process_action_button.hover_color,
        .clicked_color = (SDL_Color){0, 90, 170, 255},
        .is_clicked = view->process_action_button.is_currently_clicked,
        .is_hovered = view->process_action_button.is_currently_hovered,
        .text_color = {255, 255, 255, 255},
        .text = state->is_histogram_equalized ? "Ver original" : "Equalizar",
        .text_offset_x = state->is_histogram_equalized ? 52.0f : 68.0f,
        .text_offset_y = 16.0f};
    return props;
}

static void render_action_button_component(ApplicationView *view, const ActionButtonProps *props)
{
    SDL_Color button_color = props->normal_color;
    if (props->is_clicked)
    {
        button_color = props->clicked_color;
    }
    else if (props->is_hovered)
    {
        button_color = props->hover_color;
    }

    SDL_SetRenderDrawColor(view->secondary_renderer,
                           button_color.r,
                           button_color.g,
                           button_color.b,
                           button_color.a);
    SDL_RenderFillRect(view->secondary_renderer, &props->rect);

    if (view->ui_font)
    {
        render_text(view,
                    props->text,
                    props->rect.x + props->text_offset_x,
                    props->rect.y + props->text_offset_y,
                    props->text_color);
    }
}

void render_ui_view(ApplicationView *view, ApplicationState *state)
{
    PrimaryImageProps primary_props = create_primary_image_component();
    render_primary_image_component(view, &primary_props);

    SecondaryWindowProps secondary_props = create_secondary_window_component();
    render_secondary_window_component(view, &secondary_props);

    HistogramProps histogram_props = create_histogram_component(view);
    render_histogram_component(view, state, &histogram_props);

    AnalysisSectionProps analysis_props = create_analysis_section_component();
    render_analysis_section_component(view, state, &analysis_props);

    ActionButtonProps button_props = create_action_button_component(view, state);
    render_action_button_component(view, &button_props);

    SDL_RenderPresent(view->secondary_renderer);
}

void cleanup_ui_view(ApplicationView *view)
{
    if (view->ui_font)
    {
        TTF_CloseFont(view->ui_font);
        view->ui_font = NULL;
    }
    TTF_Quit();

    if (view->primary_image_texture)
        SDL_DestroyTexture(view->primary_image_texture);
    if (view->primary_renderer)
        SDL_DestroyRenderer(view->primary_renderer);
    if (view->primary_window)
        SDL_DestroyWindow(view->primary_window);
    if (view->secondary_renderer)
        SDL_DestroyRenderer(view->secondary_renderer);
    if (view->secondary_window)
        SDL_DestroyWindow(view->secondary_window);
}