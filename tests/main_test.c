#include <stdbool.h>
#include "tests.h"

bool add_tests(CU_pSuite suite, test_t *tests)
{
    while (tests->name != NULL) {
        if (CU_add_test(suite, tests->name, tests->func) == NULL) {
            return false;
        }

        tests++;
    }

    return true;
}

bool add_test_suite(char *name, CU_InitializeFunc pInit, CU_CleanupFunc pClean, test_t *tests)
{
    CU_pSuite pSuite = CU_add_suite(name, pInit, pClean);
    return (pSuite == NULL || !add_tests(pSuite, tests));
}

int main()
{
    // Initialize
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Bit array test suite
    if (add_test_suite("Bit Array Test Suite", init_suite_bit_array, clean_suite_bit_array,
                       BIT_ARRAY_TESTS)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Sieve utils test suite
    if (add_test_suite("Sieve Utils Test Suite", init_suite_sieve_utils, clean_suite_sieve_utils,
                       SIEVE_UTILS_TESTS)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
