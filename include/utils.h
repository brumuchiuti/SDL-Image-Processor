#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * Checks if the given filename has an allowed image extension (e.g., .png, .jpg, .bmp).
 * @param filename The name of the file to check.
 * @return true if the file has an allowed extension, false otherwise.
 */
bool has_allowed_extension(const char* filename);

#endif // UTILS_H