#ifndef __TESTS_H__
#define __TESTS_H__

#include <stdlib.h>
#include <CUnit/Basic.h>

typedef struct test
{
    const char *name;
    void (*func)(void);
} test_t;

#endif //__TESTS_H__
