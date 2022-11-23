/**
 * My own synth following this tutorial:
 * https://blog.demofox.org/2012/05/14/diy-synthesizer-chapter-1-sound-output/
 *
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "common.h"
#include "test.h"
#include "wav.h"
#include "oscillator.h"
#include "synth.h"


int main(int argc, char *argv[]) {
	int sample_rate = SAMPLE_RATE;
	int nseconds = 30;
	int nchannels = 1;
	unsigned int bar_length = 4;
	unsigned int bpm = 210;

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	/* test_all(); */
	Note notes[] = {
		new_note(3, 0, 1 << 0),
		new_note(3, 2, 1 << 1),
		/* new_note(3, 4, 1 << 2), */
		new_note(3, 5, 1 << 3),
	};

	Synth synth = new_synth(0, bar_length, *advance_osc_sine, notes, sizeof(notes) / sizeof(notes[0]));

	bar_t bar = 1;
	position_t position = 1;
	for (int i = 0; i < nsamples; i++) {
		// Get position in current bar
		int quarter_note = i * (bpm / 60) / sample_rate;
		position = 1 << (quarter_note % bar_length);
		bar = 1 << (quarter_note / bar_length);

		data[i] = advance_synth(&synth, sample_rate, position, bar);
	}

	write_wave_file("composition.wav", data, nsamples, nchannels, sample_rate);
}
