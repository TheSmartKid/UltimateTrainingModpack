#include <math.h>

#include "useful.h"

#include "hitbox_visualizer_helper.h"

float clampf(float val, float min, float max) {
	if (min >= max)
		return val >= min ? min : val <= max ? max : val;
	return val <= min ? min : val >= max ? max : val;
}

float roundf_to(float val, float align) {
	return roundf(val / align) * align;
}

float scalef(float val, float min, float max, float min2, float max2) {
	return min2 + (max2 - min2) * ((val - min) / (max - min));
}

Vector3f color_scalef(float scale, Vector3f min_color, Vector3f max_color, float gamma) {
	scale = clampf(scale, 0.0f, 1.0f);
	float gamma_inv = 1.0f / gamma;
	float align = 1.0f / 255.0f; // color components must be a multiple of 1/255
	return Vector3f{
		roundf_to(powf(scalef(scale, 0.0f, 1.0f, powf(min_color.x, gamma), powf(max_color.x, gamma)), gamma_inv), align),
		roundf_to(powf(scalef(scale, 0.0f, 1.0f, powf(min_color.y, gamma), powf(max_color.y, gamma)), gamma_inv), align),
		roundf_to(powf(scalef(scale, 0.0f, 1.0f, powf(min_color.z, gamma), powf(max_color.z, gamma)), gamma_inv), align)
	};
}
