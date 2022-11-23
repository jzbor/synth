#include <stddef.h>

#include "common.h"
#include "synth.h"
#include "oscillator.h"

Note new_note(float octave, float note, position_t pos) {
	return (Note) {
		.frequency = calc_frequency(octave, note),
		.volume_offset = 0,
		.position = pos,
		.bar = 0xffffffffffffffff,
	};
}

Synth new_synth(float volume, unsigned int bar_length,
		float (*advance_osc)(float *phase, float frequency, float sample_rate),
		Note *notes, size_t nnotes) {
	return (Synth) {
		.volume = volume,
		.phase = 0,
		.noise_last = 0.0,
		.bar_length = bar_length,
		.advance_osc = advance_osc,
		.notes = notes,
		.nnotes = nnotes,
	};
}

float advance_synth(Synth *synth, float sample_rate, position_t position, bar_t bar) {
	Note *note = NULL;
	for (int i = 0; i < synth->nnotes; i++) {
		if (synth->notes[i].position & position
				&& synth->notes[i].bar & bar) {
			note = &synth->notes[i];
		}
	}
	if (note) {
		return synth->advance_osc(&synth->phase, note->frequency, sample_rate);
	} else {
		return 0;
	}
}
