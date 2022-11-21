#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CLAMP(VAL,MIN,MAX) { if(VAL > MAX) { VAL = MAX; } else if (VAL < MIN) {VAL = MIN;} }

// specify sample bit count for wave files
// can be uint8, int16 or int32
typedef int16_t wave_sample_t;

typedef struct {
	// the main chunk
	unsigned char m_szChunkID[4];
	u_int32_t m_nChunkSize;
	unsigned char m_szFormat[4];

	// sub chunk 1 "fmt "
	unsigned char m_szSubChunk1ID[4];
	u_int32_t m_nSubChunk1Size;
	u_int16_t m_nAudioFormat;
	u_int16_t m_nNumChannels;
	u_int32_t m_nSampleRate;
	u_int32_t m_nByteRate;
	u_int16_t m_nBlockAlign;
	u_int16_t m_nBitsPerSample;

	// sub chunk 2 "data"
	unsigned char m_szSubChunk2ID[4];
	u_int32_t m_nSubChunk2Size;

	//data
} MinimalWaveFileHeader;

void die(const char *msg);
void die_no_errno(const char *msg);
u_int8_t float_to_audio_uint8(float f);
int16_t float_to_audio_int16(float f);
int32_t float_to_audio_int32(float f);
bool write_wave_header(FILE *file, int32_t data_size, int16_t nchannels, int32_t sample_rate, int32_t bits_per_sample);
bool write_wave_file(const char *file_name, float *raw_data, int32_t nsamples, u_int16_t nchannels, u_int32_t sample_rate);
bool write_wave_file_direct(const char *file_name, void * data, int32_t data_size, int16_t nchannels,
		int32_t sample_rate, int32_t bits_per_sample);
