#include "ui_components.h"

// Helper function for mouse event detection in UI components
// Checks if a point (x, y) is within the bounds of a rectangle
bool is_point_inside_rectangle(float x, float y, SDL_FRect rectangle) {
    return (x >= rectangle.x && 
            x <= (rectangle.x + rectangle.w) &&
            y >= rectangle.y && 
            y <= (rectangle.y + rectangle.h));
}