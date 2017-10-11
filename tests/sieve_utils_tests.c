#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "sieve_utils.h"

int init_suite_sieve_utils()
{
    return 0;
}

int clean_suite_sieve_utils()
{
    return 0;
}

typedef struct sieve_data {
    int lower;
    int upper;
    int remaining;
    union {
        bool *A;
        bit_array_t *array;
    };
} SieveData;

void test_sieve_parallel_bool()
{
    int T = 1000;
    int num_threads = 4;
    SieveData data[num_threads];
    int lower = 0;
    int upper = sqrt(T);
    int diff = (T - upper) / (num_threads - 1);

    for (int j = 0; j < num_threads; j++) {
        data[j].lower = lower;
        data[j].upper = upper;
        data[j].A = calloc(upper - lower, sizeof(bool));

        if (j == 0) {
            data[j].remaining = upper - lower - 1;
            memset(data[j].A + 1, true, upper - lower - 1);
        } else {
            data[j].remaining = upper - lower;
            memset(data[j].A, true, upper - lower);
        }

        lower = upper;
        upper = (T < upper + diff) ? T : upper + diff;
    }

    int i = 0;

    while (i < T) {
        i = next_prime_bool(data[0].A, i, data[0].upper);

        if (i - 1 == T) {
            break;
        }

        for (int j = 0; j < num_threads; j++) {
            data[j].remaining = remove_multiples_bool(i, data[j].A, data[j].lower, data[j].upper,
                                data[j].remaining);
        }
    }

    int count = 0;

    for (int j = 0; j < num_threads; j++) {
        count += data[j].remaining;
    }

    CU_ASSERT(count == 168);
    count = 0;

    for (int j = 0; j < num_threads; j++) {
        count += count_primes_bool(data[j].A, data[j].upper - data[j].lower);
    }

    CU_ASSERT(count == 168);

    for (int j = 0; j < num_threads; j++) {
        free(data[j].A);
    }
}

void test_sieve_parallel_bit()
{
    int T = 1000000;
    int num_threads = 4;
    SieveData data[num_threads];
    int lower = 0;
    int upper = sqrt(T);
    int diff = (T - upper) / (num_threads - 1);

    for (int j = 0; j < num_threads; j++) {
        data[j].lower = lower;
        data[j].upper = upper;
        data[j].array = bit_array_new(upper - lower);
        bit_array_fill(data[j].array);

        if (j == 0) {
            bit_array_unset(data[j].array, 0);
        }

        lower = upper;
        upper = (T < upper + diff) ? T : upper + diff;
    }

    int i = 0;

    while (i < T) {
        i = next_prime_bit(data[0].array, i, data[0].upper);

        if (i - 1 == T) {
            break;
        }

        for (int j = 0; j < num_threads; j++) {
            remove_multiples_bit(i, data[j].array, data[j].lower, data[j].upper, 0);
        }
    }

    int count = 0;

    for (int j = 0; j < num_threads; j++) {
        count += count_primes_bit(data[j].array, data[j].upper - data[j].lower);
    }

    CU_ASSERT(count == 78498);

    for (int j = 0; j < num_threads; j++) {
        bit_array_free(data[j].array);
    }
}

test_t SIEVE_UTILS_TESTS[] = {
    { "sieve_utils parallel bool", test_sieve_parallel_bool },
    { "sieve_utils parallel bit", test_sieve_parallel_bit },
    { NULL }
};
