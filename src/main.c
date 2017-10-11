#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sieve_utils.h"
#include "sieve_seq.h"
#include "sieve_par.h"

void print_usage(char *program)
{
    printf("Usage: %s <type:bit/bool> <threads> <T>\n", program);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    bool use_bit_array = strcmp(argv[1], "bit") == 0;

    if (!use_bit_array && strcmp(argv[1], "bool") != 0) {
        printf("Error: invalid array type '%s'. Available: 'bit' or 'bool'.\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[2]);
    int T = atoi(argv[3]);

    if (num_threads == 1) {
        sieve_sequential(use_bit_array, T);
    } else {
        sieve_parallel(use_bit_array, T, num_threads);
    }

    return 0;
}
