#ifndef USEFUL_VISUAL_H
#define USEFUL_VISUAL_H

#include "useful.h"

/**
 * Restricts a float to the range [min, max].
 */
float clampf(float val, float min, float max);

/**
 * Rounds a number to the nearest multiple of another number.
 */
float roundf_to(float val, float align);

/**
 * Converts a float from one scale to another.
 * For example, one could convert Fahrenheit temperatures to Celsius with <code>scale(temperature, 32.0f, 212.0f, 0.0f, 100.0f)</code>.
 * If the last two arguments are not specified, they are assumed to be 0 and 1.
 */
float scalef(float val, float min, float max, float min2 = 0.0f, float max2 = 1.0f);

/**
 * Same as scalef(), but the final value is always in the range [min2, max2].
 */
float scalef_bounded(float val, float min, float max, float min2 = 0.0f, float max2 = 1.0f);

/**
 * Linearly nterpolates between two colors, with bounds checking, accounting for gamma.
 * arguments:
 * - min_color (Vector3f) -- xyz maps to rgb, components are usually in the range [0.0f, 1.0f] but can go beyond to account for super-bright or super-dark colors
 * - max_Color (Vector3f) -- same as minColor
 * - t (float) -- how far to interpolate between the colors
 * - gamma (float = 2.0f) -- used for color correction, helps avoid ugly dark colors when interpolating b/t bright colors
 */
Vector3f color_scalef(float scale, Vector3f min_color, Vector3f max_color, float gamma = 2.0f);
	
#endif // USEFUL_VISUAL_H
