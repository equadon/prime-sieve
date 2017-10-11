#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "bit_array.h"

#define BYTE_SIZE 8

static bit_array_t *array;

static const int ARR_SIZE = 3;
static const int SIZE = 3 * BYTE_SIZE;

int init_suite_bit_array()
{
    array = bit_array_new(SIZE);
    return (array == NULL) ? -1 : 0;
}

int clean_suite_bit_array()
{
    bit_array_free(array);
    return 0;
}

void test_bit_set()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_test(array, 7) == false);
    CU_ASSERT(bit_array_test(array, 8) == false);
    CU_ASSERT(bit_array_test(array, 9) == false);
    bit_array_set(array, 7);
    bit_array_set(array, 8);
    bit_array_set(array, 9);
    CU_ASSERT(bit_array_test(array, 7) == true);
    CU_ASSERT(bit_array_test(array, 8) == true);
    CU_ASSERT(bit_array_test(array, 9) == true);
    bit_array_set(array, 8);
    CU_ASSERT(bit_array_test(array, 7) == true);
    CU_ASSERT(bit_array_test(array, 8) == true);
    CU_ASSERT(bit_array_test(array, 9) == true);
    bit_array_unset(array, 8);
    CU_ASSERT(bit_array_test(array, 7) == true);
    CU_ASSERT(bit_array_test(array, 8) == false);
    CU_ASSERT(bit_array_test(array, 9) == true);
}

void test_bit_unset()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_test(array, 7) == false);
    CU_ASSERT(bit_array_test(array, 8) == false);
    CU_ASSERT(bit_array_test(array, 9) == false);
    bit_array_unset(array, 8);
    CU_ASSERT(bit_array_test(array, 7) == false);
    CU_ASSERT(bit_array_test(array, 8) == false);
    CU_ASSERT(bit_array_test(array, 9) == false);
    bit_array_set(array, 8);
    CU_ASSERT(bit_array_test(array, 7) == false);
    CU_ASSERT(bit_array_test(array, 8) == true);
    CU_ASSERT(bit_array_test(array, 9) == false);
    bit_array_unset(array, 8);
    CU_ASSERT(bit_array_test(array, 7) == false);
    CU_ASSERT(bit_array_test(array, 8) == false);
    CU_ASSERT(bit_array_test(array, 9) == false);
}

void test_bit_toggle()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_test(array, 11) == false);
    CU_ASSERT(bit_array_test(array, 12) == false);
    CU_ASSERT(bit_array_test(array, 13) == false);
    bit_array_toggle(array, 12);
    CU_ASSERT(bit_array_test(array, 11) == false);
    CU_ASSERT(bit_array_test(array, 12) == true);
    CU_ASSERT(bit_array_test(array, 13) == false);
    bit_array_toggle(array, 12);
    CU_ASSERT(bit_array_test(array, 11) == false);
    CU_ASSERT(bit_array_test(array, 12) == false);
    CU_ASSERT(bit_array_test(array, 13) == false);
}

void test_bit_test()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_test(array, 3) == false);
    bit_array_set(array, 3);
    CU_ASSERT(bit_array_test(array, 3) == true);
    CU_ASSERT(bit_array_test(array, 11) == false);
    bit_array_set(array, 11);
    CU_ASSERT(bit_array_test(array, 11) == true);
}

void test_bit_reset()
{
    bit_array_set(array, 1);
    bool all_zeros = true;

    for (int i = 0; i < SIZE; i++)
        if (bit_array_test(array, i)) {
            all_zeros = false;
        }

    CU_ASSERT(all_zeros == false);
    bit_array_clear(array);
    all_zeros = true;

    for (int i = 0; i < SIZE; i++)
        if (bit_array_test(array, i)) {
            all_zeros = false;
        }

    CU_ASSERT(all_zeros == true);
}

void test_bit_fill()
{
    bit_array_clear(array);
    bool all_ones = true;

    for (int i = 0; i < SIZE; i++)
        if (!bit_array_test(array, i)) {
            all_ones = false;
        }

    CU_ASSERT(all_ones == false);
    bit_array_fill(array);
    all_ones = true;

    for (int i = 0; i < SIZE; i++)
        if (!bit_array_test(array, i)) {
            all_ones = false;
        }

    CU_ASSERT(all_ones == true);
}

void test_bit_length()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_length(array) == SIZE);
}

void test_bit_count()
{
    bit_array_clear(array);
    CU_ASSERT(bit_array_count(array) == 0);
    bit_array_set(array, 3);
    bit_array_set(array, 5);
    CU_ASSERT(bit_array_count(array) == 2);
    bit_array_set(array, 7);
    bit_array_set(array, 9);
    CU_ASSERT(bit_array_count(array) == 4);
}

test_t BIT_ARRAY_TESTS[] = {
    { "bit_array set", test_bit_set },
    { "bit_array unset", test_bit_unset },
    { "bit_array toggle", test_bit_toggle },
    { "bit_array test", test_bit_test },
    { "bit_array reset", test_bit_reset },
    { "bit_array fill", test_bit_fill },
    { "bit_array get_length", test_bit_length },
    { "bit_array count", test_bit_count },
    { NULL }
};
