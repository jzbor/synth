#pragma once

#include <stdbool.h>

float calc_frequency(float octave, float note);
float advance_osc_sine(float *phase, float frequency, float sample_rate);
float advance_osc_square(float *phase, float frequency, float sample_rate);
float advance_osc_triangle(float *phase, float frequency, float sample_rate);
float advance_osc_noise(float *phase, float frequency, float sample_rate, float last_value);
float advance_osc_noise_intense(float *phase, float frequency, float sample_rate, float last_value);
