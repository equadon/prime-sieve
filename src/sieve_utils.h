#ifndef __SIEVE_UTILS_H__
#define __SIEVE_UTILS_H__

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "bit_array.h"

// https://gist.github.com/zeta709/6208314
#define timespec_sub(a, b, result) \
    do { \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
        (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
        if ((result)->tv_nsec < 0) { \
            --(result)->tv_sec; \
            (result)->tv_nsec += 1000000000; \
        } \
    } while (0)

// Start timer
#define timer_start() \
    ({ \
        clock_gettime(CLOCK_REALTIME, &time_start); \
    })

// Stop timer
#define timer_stop() \
    ({ \
        clock_gettime(CLOCK_REALTIME, &time_end); \
        timespec_sub(&time_end, &time_start, &time_diff); \
        printf("Elapsed time: %ld.%09ld second(s)\n", time_diff.tv_sec, time_diff.tv_nsec); \
    })

typedef enum array_type {
    BIT_ARRAY,
    BOOL_ARRAY
} array_type_t;

void print_bit_array(bit_array_t *array, unsigned int T);
void print_bool_array(bool A[], unsigned int lower, unsigned int upper);

unsigned int next_prime_bit(bit_array_t *array, unsigned int i, unsigned int T);
unsigned int next_prime_bool(bool A[], unsigned int i, unsigned int T);

int remove_multiples_bit(uint32_t multiple, bit_array_t *array, uint32_t lower, uint32_t upper,
                         int remaining);
int remove_multiples_bool(uint32_t multiple, bool A[], uint32_t lower, uint32_t upper,
                          int remaining);

int count_primes_bit(bit_array_t *array, int length);
int count_primes_bool(bool A[], uint32_t length);

int min(int a, int b);
int max(int a, int b);

#endif //__SIEVE_UTILS_H__
