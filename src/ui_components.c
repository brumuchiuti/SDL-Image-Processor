#include "ui_components.h"

bool is_point_inside_rectangle(float x, float y, SDL_FRect rectangle) {
    return (x >= rectangle.x && 
            x <= (rectangle.x + rectangle.w) &&
            y >= rectangle.y && 
            y <= (rectangle.y + rectangle.h));
}