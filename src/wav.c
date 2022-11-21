#include <stdlib.h>

#include "wav.h"

// 0 to 255
u_int8_t float_to_audio_uint8(float f) {
	f = (f + 1.0f) * 127.5f;
	CLAMP(f, 0.0f, 255.0f);
	return (u_int8_t) f;
}

// –32,768 to 32,767
int16_t float_to_audio_int16(float f) {
	f *= 32767.0f;
	CLAMP(f, -32768.0f, 32767.0f);
	return (int16_t) f;
}

// –2,147,483,648 to 2,147,483,647
int32_t float_to_audio_int32(float f) {
	double d = (double) f;
	d *= 2147483647.0;
	CLAMP(d, -2147483648.0, 2147483647.0);
	return (int32_t) d;
}


bool write_wave_header(FILE *file, int32_t data_size, int16_t nchannels, int32_t sample_rate, int32_t bits_per_sample) {
	MinimalWaveFileHeader header = {0};

	// fill out main chunk
	memcpy(header.m_szChunkID, "RIFF", 4);
	header.m_nChunkSize = data_size + 36;
	memcpy(header.m_szFormat, "WAVE", 4);

	// fill out sub chunk 1 "fmt "
	memcpy(header.m_szSubChunk1ID, "fmt ", 4);
	header.m_nSubChunk1Size = 16;
	header.m_nAudioFormat = 1;
	header.m_nNumChannels = nchannels;
	header.m_nSampleRate = sample_rate;
	header.m_nByteRate = sample_rate * nchannels * bits_per_sample / 8;
	header.m_nBlockAlign = nchannels * bits_per_sample / 8;
	header.m_nBitsPerSample = bits_per_sample;

	// fill out sub chunk 2 "data"
	memcpy(header.m_szSubChunk2ID, "data", 4);
	header.m_nSubChunk2Size = data_size;

	// write the header
	fwrite(&header, sizeof(header), 1, file);

	return true;
}

bool write_wave_file(const char *file_name, float *raw_data, int32_t nsamples, u_int16_t nchannels, u_int32_t sample_rate) {
	// convert data to int
	wave_sample_t data[nsamples];
	for (int i = 0; i < nsamples; i++) {
		switch (sizeof(wave_sample_t)) {
			case sizeof(u_int8_t):
				data[i] = float_to_audio_uint8(raw_data[i]);
				break;
			case sizeof(int16_t):
				data[i] = float_to_audio_int16(raw_data[i]);
				break;
			case sizeof(int32_t):
				data[i] = float_to_audio_int32(raw_data[i]);
				break;
			default:
				die_no_errno("Invalid wav sample size");
		}
	}

	// calculate bits per sample and the data size
	u_int32_t bits_per_sample = sizeof(wave_sample_t) * 8;
	u_int32_t data_size = nsamples * sizeof(wave_sample_t);

	return write_wave_file_direct(file_name, data, data_size, nchannels, sample_rate, bits_per_sample);
}

bool write_wave_file_direct(const char *file_name, void * data, int32_t data_size, int16_t nchannels,
		int32_t sample_rate, int32_t bits_per_sample) {
	// open file if possible
	FILE *file = fopen(file_name, "w+b");
	if (!file) {
		return false;
	}

	// write header
	write_wave_header(file, data_size, nchannels, sample_rate, bits_per_sample);

	// write data
	fwrite(data, data_size, 1, file);

	// close file and return success
	fclose(file);
	return true;

	// @TODO add error handling for file io
}
