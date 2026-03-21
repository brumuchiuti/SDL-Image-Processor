#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <SDL3/SDL.h>
#include <stdbool.h>

/**
 * Checks if the given surface is grayscale by verifying that all pixels have the same R, G, and B values.
 * @param surface The SDL_Surface to check.
 * @return true if the surface is grayscale, false otherwise.
 */
bool is_grayscale(SDL_Surface *surface);

/**
 * Converts the given surface to grayscale using the luminosity method, which calculates the luminance of each pixel and sets the R, G, and B values to that luminance value.
 * @param surface The SDL_Surface to convert to grayscale.
 * The formula used for calculating luminance is: Y = 0.2125 * R + 0.7154 * G + 0.0721 * B
 */
void convert_to_grayscale(SDL_Surface *surface);

/**
 * Calculates the frequency of each shade of gray and stores it in the provided array.
 * @param surface The SDL_Surface for which to calculate the histogram.
 * @param histogram An array of size 256 to store the frequency of each gray level.
 */
void calculate_histogram(SDL_Surface *surface, int histogram[256]);

/**
 * Equalizes the grayscale histogram of the given RGBA surface in-place.
 * @param surface The grayscale SDL_Surface to equalize.
 */
void equalize_histogram(SDL_Surface *surface);

#endif // IMAGE_PROCESSOR_H