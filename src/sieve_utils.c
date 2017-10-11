#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "sieve_utils.h"

void print_bit_array(bit_array_t *array, unsigned int T)
{
    printf("Primes: [ ");

    for (int i = 0; i < T; i++) {
        if (bit_array_test(array, i)) {
            printf("%d ", i + 1);
        }
    }

    printf("]\n");
}

void print_bool_array(bool A[], unsigned int lower, unsigned int upper)
{
    unsigned int T = upper - lower;
    printf("Primes: [ ");

    for (int i = 0; i < T; i++) {
        if (A[i]) {
            printf("%d ", lower + i + 1);
        }
    }

    printf("]\n");
}

unsigned int next_prime_bit(bit_array_t *array, unsigned int i, unsigned int T)
{
    while (i < T && !bit_array_test(array, i)) {
        i++;
    }

    return i + 1;
}

unsigned int next_prime_bool(bool A[], unsigned int i, unsigned int T)
{
    while (i < T && !A[i]) {
        i++;
    }

    return i + 1;
}

int remove_multiples_bit(uint32_t multiple, bit_array_t *array, uint32_t lower, uint32_t upper,
                         int remaining)
{
    uint32_t j = max(2, ceil((double)(lower + 1) / (double) multiple));
    uint32_t length = bit_array_length(array);
    int k = j * multiple - 1 - lower;

    while (k < length) {
        bit_array_unset(array, j * multiple - 1 - lower);
        j++;
        k = j * multiple - 1 - lower;
    }

    return remaining;
}

int remove_multiples_bool(uint32_t multiple, bool A[], uint32_t lower, uint32_t upper,
                          int remaining)
{
    uint32_t j = max(2, ceil((double)(lower + 1) / (double) multiple));
    int k = j * multiple - 1 - lower;

    while (k < upper - lower) {
        if (A[k]) {
            A[k] = false;
            remaining--;
        }

        j++;
        k = j * multiple - 1 - lower;
    }

    return remaining;
}

int count_primes_bit(bit_array_t *array, int length)
{
    /*return bit_array_count(array);*/
    int count = 0;

    for (int i = 0; i < length; i++)
        if (bit_array_test(array, i)) {
            count++;
        }

    return count;
}

int count_primes_bool(bool A[], uint32_t length)
{
    int count = 0;

    for (int i = 0; i < length; i++)
        if (A[i]) {
            count++;
        }

    return count;
}

int min(int a, int b)
{
    return (b < a) ? b : a;
}

int max(int a, int b)
{
    return (b > a) ? b : a;
}
