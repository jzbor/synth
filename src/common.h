#pragma once

// MACRO DEFINITIONS
#define DBGPRINT(fmt, ...) do { \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        } while (0)

// TYPE AND VALUE DEFINITIONS
#define SAMPLE_RATE 	44100
#define TEST_DURATION 	10
#define PI 		(acos(-1.0))


