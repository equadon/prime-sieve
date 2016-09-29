#include <stdbool.h>
#include "tests.h"

bool add_tests(CU_pSuite suite, test_t *tests)
{
    do {
        if (CU_add_test(suite, tests->name, tests->func) == NULL)
            return false;
        tests++;
    } while (tests->name != NULL);

    return true;
}

int main()
{
    // Initialize
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add suites

    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
