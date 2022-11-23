#pragma once

#include <sys/types.h>

typedef u_int32_t position_t;
typedef u_int64_t bar_t;

typedef struct {
	float frequency;
	float volume_offset;
	position_t position;
	bar_t bar;
} Note;

typedef struct {
	float volume;
	float phase;
	float noise_last;
	unsigned int bar_length;
	float (*advance_osc)(float *phase, float frequency, float sample_rate);
	Note *notes;
	size_t nnotes;
} Synth;


Note new_note(float octave, float note, position_t pos);
Synth new_synth(float volume, unsigned int bar_length,
		float (*advance_osc)(float *phase, float frequency, float sample_rate),
		Note *notes, size_t nnotes);
float advance_synth(Synth *synth, float sample_rate, position_t position, bar_t bar);


