#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * Checks if the given filename has an allowed image extension (e.g., .png, .jpg, .bmp).
 * @param filename The name of the file to check.
 * @return true if the file has an allowed extension, false otherwise.
 */
bool has_allowed_extension(const char* filename);

/**
 * Converts frames per second (FPS) to a delay in milliseconds, which can be used to control the rendering speed of the application.
 * @param fps The desired frames per second. If fps is less than or equal to 0, a default delay of 16 ms (approximately 60 FPS) is returned.
 * @return The delay in milliseconds.
 */
int from_fps_to_delay_in_ms(int fps);

#endif // UTILS_H