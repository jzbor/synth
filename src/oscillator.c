#include <math.h>

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
