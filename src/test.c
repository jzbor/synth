#include <sys/types.h>
#include <math.h>

#include "test.h"
#include "common.h"
#include "wav.h"
#include "oscillator.h"

void test_all() {
	test_ch01_mono_sawtooth();
	test_ch01_stereo_sawtooth();
	test_ch02_naive_sine();
	test_ch02_discontinuous_sine();
	test_ch02_continuous_sine();
	test_ch02_vanilla_sine();
	test_ch02_quiet_sine();
	test_ch02_clipping_sine();
	test_ch02_square();
	test_ch02_triangle();
	test_ch02_noise();
	test_ch02_noise_intense();
}

void test_ch01_mono_sawtooth() {
	int sample_rate = 44100;
	int nseconds = 4;
	int nchannels = 1;

	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	// Create sawtooth by increasing int until overflow
	int32_t val = 0;
	for (int i = 0; i < nsamples; i++) {
		val += 8000000;
		data[i] = val;
	}

	write_wave_file_direct("outmono.wav", data, nsamples * sizeof(data[0]), nchannels, sample_rate, sizeof(data[0]) * 8);
}

void test_ch01_stereo_sawtooth() {
	int sample_rate = 44100;
	int nseconds = 4;
	int nchannels = 2;

	int nsamples = sample_rate * nchannels * nseconds;
	u_int32_t data[nsamples];

	int32_t val1 = 0;
	int32_t val2 = 0;
	for (int i = 0; i < nsamples; i += 2) {
		val1 +=  8000000;
		val2 += 12000000;
		data[i] = val1;
		data[i + 1] = val2;
	}

	write_wave_file_direct("outstereo.wav", data, nsamples * sizeof(data[0]), nchannels, sample_rate, sizeof(data[0]) * 8);
}

void test_ch02_naive_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float frequency = calc_frequency(1, 3); // middle C

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = sin((float) i * 2 * (float) PI * frequency / (float) sample_rate);
	}

	write_wave_file("sinenaive.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_discontinuous_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		if (i < nsamples / 2) {
			float cur_frequency = calc_frequency(3, 3);
			data[i] = sin((float) i * 2 * (float) PI * cur_frequency / (float) sample_rate);
		} else {
			float cur_frequency = calc_frequency(3, 4);
			data[i] = sin((float) i * 2 * (float) PI * cur_frequency / (float) sample_rate);
		}
	}

	write_wave_file("sinediscon.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_continuous_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		if (i < nsamples / 2) {
			float cur_frequency = calc_frequency(3, 3);
			phase += 2 * (float) PI * cur_frequency / (float) sample_rate;

		} else {
			float cur_frequency = calc_frequency(3, 4);
			phase += 2 * (float) PI * cur_frequency / (float) sample_rate;
		}

		// limit phase between 0 and 2*PI to avoid inaccurate floats at high numbers
		// while is used instead of if to avoid passing the thresholds multiple times
		// 	on high frequencies
		while (phase >= 2 * (float) PI) {
			phase -= 2 * (float) PI;
		}
		while (phase < 0) {
			phase += 2 * (float) PI;
		}

		data[i] = sin(phase);
	}

	write_wave_file("sinecon.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_vanilla_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_sine(&phase, frequency, (float) sample_rate);
	}

	write_wave_file("sine.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_quiet_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_sine(&phase, frequency, (float) sample_rate) * 0.4f;
	}

	write_wave_file("sinequiet.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_clipping_sine() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_sine(&phase, frequency, (float) sample_rate) * 1.4f;
	}

	write_wave_file("sineclip.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_square() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_square(&phase, frequency, (float) sample_rate);
	}

	write_wave_file("square.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_triangle() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_triangle(&phase, frequency, (float) sample_rate);
	}

	write_wave_file("triangle.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_noise() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_noise(&phase, frequency, (float) sample_rate, i > 0 ? data[i - 1] : 0.0f);
	}

	write_wave_file("noise.wav", data, nsamples, nchannels, sample_rate);
}

void test_ch02_noise_intense() {
	int sample_rate = SAMPLE_RATE;
	int nseconds = TEST_DURATION;
	int nchannels = 1;
	float phase = 0;
	float frequency = calc_frequency(3, 3);

	// create buffer for samples
	int nsamples = sample_rate * nchannels * nseconds;
	float data[nsamples];

	for (int i = 0; i < nsamples; i++) {
		data[i] = advance_osc_noise_intense(&phase, frequency, (float) sample_rate, i > 0 ? data[i - 1] : 0.0f);
	}

	write_wave_file("noiseintense.wav", data, nsamples, nchannels, sample_rate);
}
