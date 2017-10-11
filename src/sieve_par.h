#ifndef __SIEVE_PAR_H__
#define __SIEVE_PAR_H__

#include <stdbool.h>

void sieve_parallel(bool use_bit_array, unsigned int T, int num_threads);

#endif //__SIEVE_PAR_H__
