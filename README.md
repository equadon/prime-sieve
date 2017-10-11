# Sieve of Eratosthenes using POSIX Threads

`bin/primes <array type> <threads> <T>`

## Compile

Compile the program with debugging symbols:

`make` or `make debug`

Compile the program with optimizations and no debugging symbols:

`make release`

Clean temporary build files:

`make clean`

## Benchmark

The benchmarking tool tests both strong and weak scalability with both array types. Each test is executed several times and the best and worst times are removed then the mean is used as the final execution time. The test results are collected and plotted using `gnuplot`.

Run the benchmark tool:

`make bench`

Run the benchmark tool with `T=4194304`, **2** cores and **8** runs per test:

`make T=4194304 CORES=2 NUMTESTS=8 bench`

## Examples

Count the number of primes less than or equal to **1 000 000** with the **bit** array and **4** threads using the parallel sieve:

`bin/primes bit 4 1000000`

Count the number of primes less than or equal to **512** using the **Boolean** array and the sequential prime sieve:

`bin/primes bool 1 512`

## Tests

Compile and run unit tests:

`make test`

Run tests and generate a code coverage report using LCOV in `doc/coverage`:

`make cov`

## Documentation

Generate documentation with *Doxygen* in `doc/`:

`make doc`

