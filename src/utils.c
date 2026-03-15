#include "utils.h"
#include <string.h>

static const char* ALLOWED_EXTENSIONS[] = {
    "png",
    "jpg",
    "bmp"
};
static const int NUM_ALLOWED_EXTENSIONS = sizeof(ALLOWED_EXTENSIONS) / sizeof(ALLOWED_EXTENSIONS[0]);

bool has_allowed_extension(const char* filename) {
    const char* extension = strrchr(filename, '.');
    if (!extension || extension == filename) {
        return false;
    }

    extension++; // ignore the dot

    for (int i = 0; i < NUM_ALLOWED_EXTENSIONS; i++) {
        if (strcmp(extension, ALLOWED_EXTENSIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}

int from_fps_to_delay_in_ms(int fps) {
    if (fps <= 0) {
        return 16; // Default to approximately 60 FPS
    }
    return 1000 / fps;
}