#include <math.h>
#include <stdlib.h>

#include "oscillator.h"
#include "common.h"

//calculate the frequency of the specified note.
//fractional notes allowed!
float calc_frequency(float octave, float note) {
	/*
	       Calculate the frequency of any note!
	       frequency = 440×(2^(n/12))
	    
	       N=0 is A4
	       N=1 is A#4
	       etc...
	    
	       notes go like so...
	       0  = A
	       1  = A#
	       2  = B
	       3  = C
	       4  = C#
	       5  = D
	       6  = D#
	       7  = E
	       8  = F
	       9  = F#
	       10 = G
	       11 = G#
	   */
	return (float)(440 * pow(2.0, ((double) ((octave - 4) * 12 + note)) / 12.0));
}

float advance_osc_sine(float *phase, float frequency, float sample_rate) {
	*phase += 2 * (float) PI * frequency / sample_rate;

	// limit phase between 0 and 2*PI to avoid inaccurate floats at high numbers
	// while is used instead of if to avoid passing the thresholds multiple times
	// 	on high frequencies
	while (*phase >= 2 * (float) PI) {
		*phase -= 2 * (float) PI;
	}
	while (*phase < 0) {
		*phase += 2 * (float) PI;
	}

	return sin(*phase);
}

float advance_osc_saw(float *phase, float frequency, float sample_rate) {
	*phase += frequency / sample_rate;

	while (*phase > 1.0f) {
		*phase -= 1.0f;
	}
	while (*phase < 0.0f) {
		*phase += 1.0f;
	}

	return (*phase * 2.0f) - 1.0f;
}

float advance_osc_square(float *phase, float frequency, float sample_rate) {
	*phase += frequency / sample_rate;

	while (*phase > 1.0f) {
		*phase -= 1.0f;
	}
	while (*phase < 0.0f) {
		*phase += 1.0f;
	}

	if (*phase <= 0.5f) {
		return -1.0f;
	} else {
		return 1.0f;
	}
}

float advance_osc_triangle(float *phase, float frequency, float sample_rate) {
	*phase += frequency / sample_rate;

	while (*phase > 1.0f) {
		*phase -= 1.0f;
	}
	while (*phase < 0.0f) {
		*phase += 1.0f;
	}

	float ret;
	if (*phase <= 0.5f) {
		ret = *phase * 2;
	} else {
		ret = (1.0f - *phase) * 2;
	}

	return (ret * 2.0f) - 1.0f;
}

float advance_osc_noise_helper(float *phase, float frequency, float sample_rate, float last_value, bool high_intensity) {
	unsigned int last_seed = (unsigned int) *phase;
	*phase += frequency / sample_rate;
	unsigned int seed = (unsigned int) *phase;

	while (*phase > 2.0f) {
		*phase -= 1.0f;
	}

	if (seed != last_seed) {
		float val = ((float) rand()) / ((float) RAND_MAX);
		val = (val * 2.0f) - 1.0f;

		if (high_intensity) {
			if (val < 0) {
				val = -1.0f;
			} else {
				val = 1.0f;
			}
		}

		return val;
	} else {
		return last_value;
	}
}

float advance_osc_noise(float *phase, float frequency, float sample_rate, float last_value) {
	return advance_osc_noise_helper(phase, frequency, sample_rate, last_value, false);
}

float advance_osc_noise_intense(float *phase, float frequency, float sample_rate, float last_value) {
	return advance_osc_noise_helper(phase, frequency, sample_rate, last_value, true);
}
