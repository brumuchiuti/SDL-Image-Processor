#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_FRect boundary_rectangle;
    SDL_Color normal_color;
    SDL_Color hover_color;
    bool is_currently_hovered;
    bool is_currently_clicked;
} ButtonComponent;

typedef struct {
    SDL_FRect boundary_rectangle;
    SDL_Color background_color;
    SDL_Color bar_color;
} HistogramComponent;

/**
 * Check if a point (x, y) is inside a given rectangle.
 * @param x The x-coordinate of the point to check.
 * @param y The y-coordinate of the point to check.
 * @param rectangle The SDL_FRect structure representing the rectangle.
 * @return true if the point is inside the rectangle, false otherwise.
 */
bool is_point_inside_rectangle(float x, float y, SDL_FRect rectangle);

#endif // UI_COMPONENTS_H