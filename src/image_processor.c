#include "image_processor.h"
#include <string.h>

bool is_grayscale(SDL_Surface *surface)
{
    Uint8 *pixels = (Uint8 *)surface->pixels;

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            int offset = (y * surface->pitch) + (x * 4);

            // Uint8 = unsigned char (8 bits) = 0-255
            Uint8 r = pixels[offset + 0];
            Uint8 g = pixels[offset + 1];
            Uint8 b = pixels[offset + 2];

            if (r != g || r != b)
            {
                return false;
            }
        }
    }
    return true;
}

void convert_to_grayscale(SDL_Surface *surface)
{
    Uint8 *pixels = (Uint8 *)surface->pixels;

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            int offset = (y * surface->pitch) + (x * 4);
            Uint8 r = pixels[offset + 0];
            Uint8 g = pixels[offset + 1];
            Uint8 b = pixels[offset + 2];

            // Formula: Y = 0.2125 * R + 0.7154 * G + 0.0721 * B
            float luminance = (0.2125f * r) + (0.7154f * g) + (0.0721f * b);

            Uint8 grayScaleValue = (Uint8)luminance;

            pixels[offset + 0] = grayScaleValue;
            pixels[offset + 1] = grayScaleValue;
            pixels[offset + 2] = grayScaleValue;
        }
    }
}

void calculate_histogram(SDL_Surface *surface, int histogram[256])
{
    memset(histogram, 0, sizeof(int) * 256);
    if (!surface)
        return;

    Uint8 *pixels = (Uint8 *)surface->pixels;
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            int offset = (y * surface->pitch) + (x * 4);
            Uint8 gray = pixels[offset]; // R == G == B after grayscale conversion
            histogram[gray]++;
        }
    }
}

void equalize_histogram(SDL_Surface *surface)
{
    if (!surface)
        return;

    int histogram[256];

    // we need the current histogram to compute the equalization mapping function (LUT)
    calculate_histogram(surface, histogram);

    int total_pixels = surface->w * surface->h;
    if (total_pixels <= 0)
        return;

    int cdf[256];
    int cumulative = 0;
    for (int i = 0; i < 256; i++)
    {
        cumulative += histogram[i];
        cdf[i] = cumulative;
    }

    int cdf_min = 0;
    for (int i = 0; i < 256; i++)
    {
        if (cdf[i] > 0)
        {
            cdf_min = cdf[i];
            break;
        }
    }

    if (cdf_min == total_pixels)
        return;

    Uint8 lut[256];
    for (int i = 0; i < 256; i++)
    {
        float normalized = ((float)(cdf[i] - cdf_min) / (float)(total_pixels - cdf_min)) * 255.0f;
        if (normalized < 0.0f)
            normalized = 0.0f;
        if (normalized > 255.0f)
            normalized = 255.0f;
        lut[i] = (Uint8)(normalized + 0.5f);
    }

    Uint8 *pixels = (Uint8 *)surface->pixels;
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            int offset = (y * surface->pitch) + (x * 4);
            Uint8 gray = pixels[offset];
            Uint8 mapped = lut[gray];

            pixels[offset + 0] = mapped;
            pixels[offset + 1] = mapped;
            pixels[offset + 2] = mapped;
        }
    }
}