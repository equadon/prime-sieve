#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sieve_seq.h"
#include "sieve_utils.h"

int sieve_sequential_bit(unsigned int T)
{
#if RELEASE
    struct timespec time_start, time_end, time_diff;
    timer_start();
#endif
    unsigned int count;
    bit_array_t *array = bit_array_new(T);
    bit_array_fill(array);
    bit_array_unset(array, 0);
    unsigned int i = 0;
    int remaining = T - 1;

    while (i < T) {
        i = next_prime_bit(array, i, T);

        if (i - 1 == T) {
            break;
        }

        remove_multiples_bit(i, array, 0, T, remaining);
    }

    count = bit_array_count(array);
#if RELEASE
    timer_stop();
#endif
#ifdef DEBUG
    print_bit_array(array, T);
#endif
    bit_array_free(array);
    return count;
}

int sieve_sequential_bool(unsigned int T)
{
#if RELEASE
    struct timespec time_start, time_end, time_diff;
    timer_start();
#endif
    unsigned int count = T - 1;
    bool *A = calloc(T, sizeof(bool));
    memset(A + 1, true, T - 1);
    unsigned int i = 0;

    while (i < T) {
        i = next_prime_bool(A, i, T);

        if (i - 1 == T) {
            break;
        }

        count = remove_multiples_bool(i, A, 0, T, count);
    }

#if RELEASE
    timer_stop();
#endif
#ifdef DEBUG
    print_bool_array(A, 0, T);
#endif
    free(A);
    return count;
}

void sieve_sequential(bool use_bit_array, unsigned int T)
{
    int count = 0;

    if (use_bit_array) {
        count = sieve_sequential_bit(T);
    } else {
        count = sieve_sequential_bool(T);
    }

    printf("Prime count: %d\n", count);
}
