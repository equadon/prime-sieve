#ifndef __TESTS_H__
#define __TESTS_H__

#include <stdlib.h>
#include <CUnit/Basic.h>

typedef struct test {
    const char *name;
    void (*func)(void);
} test_t;

/*
 * Bit array test functions.
 */
int init_suite_bit_array();
int clean_suite_bit_array();

extern test_t BIT_ARRAY_TESTS[];

/*
 * Sieve utils test functions.
 */
int init_suite_sieve_utils();
int clean_suite_sieve_utils();

extern test_t SIEVE_UTILS_TESTS[];

#endif //__TESTS_H__
