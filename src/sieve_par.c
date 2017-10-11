#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sieve_utils.h"
#include "sieve_par.h"

#define SECOND 1000000

typedef struct thread_data {
    int id;
    int num_threads;
    array_type_t type;
    int lower;
    int upper;
    int remaining;
    union {
        bit_array_t *array;
        bool *A;
    };
} ThreadData;

/*
 * Shared variables
 */
int prime_count;
unsigned int next_prime;
int read_count;
bool main_finished;

// PThread mutex, barrier and condition variables
pthread_mutex_t lock;
pthread_barrier_t barrier;
pthread_cond_t main_cond;
pthread_cond_t worker_cond;

void print(ThreadData *data, char *message)
{
    if (data->id == 0) {
        printf("[main] %s\n", message);
    } else {
        printf("[worker %d] %s\n", data->id, message);
    }
}

void *sieve_main(void *arg)
{
    ThreadData *data = arg;
    bool is_bit = data->type == BIT_ARRAY;
#ifdef DEBUG
    print(data, "started");
#endif
    pthread_mutex_lock(&lock);
    read_count = data->num_threads - 1;
    pthread_barrier_wait(&barrier);
    pthread_cond_wait(&main_cond, &lock);
    unsigned int i = 0;

    while (i < data->upper) {
        if (is_bit) {
            i = next_prime_bit(data->array, i, data->upper);
        } else {
            i = next_prime_bool(data->A, i, data->upper);
        }

        if (i - 1 == data->upper) {
            break;
        }

        next_prime = i;
        read_count = data->num_threads - 1;

        if (is_bit) {
            data->remaining = remove_multiples_bit(i, data->array, data->lower, data->upper, data->remaining);
        } else {
            data->remaining = remove_multiples_bool(i, data->A, data->lower, data->upper, data->remaining);
        }

#ifdef DEBUG
        print(data, "found the next prime!");
#endif
        pthread_cond_broadcast(&worker_cond);
#ifdef DEBUG
        print(data, "waiting for workers...");
#endif
        pthread_cond_wait(&main_cond, &lock);
    }

    if (is_bit) {
        prime_count += bit_array_count(data->array);
    } else {
        prime_count += data->remaining;
    }

    pthread_mutex_unlock(&lock);
    main_finished = true;
    pthread_cond_broadcast(&worker_cond);
#ifdef DEBUG
    print(data, "finished");
#endif
    pthread_exit(NULL);
}

void *sieve_worker(void *arg)
{
    ThreadData *data = arg;
    bool is_bit = data->type == BIT_ARRAY;
#ifdef DEBUG
    print(data, "started");
#endif
    pthread_barrier_wait(&barrier);
    pthread_mutex_lock(&lock);

    while (1) {
        read_count--;

        if (read_count < 1) {
            pthread_cond_signal(&main_cond);
        }

        if (main_finished) {
            break;
        }

#ifdef DEBUG
        print(data, "waiting for prime...");
#endif
        pthread_cond_wait(&worker_cond, &lock);

        if (is_bit) {
            data->remaining = remove_multiples_bit(next_prime, data->array, data->lower, data->upper,
                                                   data->remaining);
        } else {
            data->remaining = remove_multiples_bool(next_prime, data->A, data->lower, data->upper,
                                                    data->remaining);
        }
    }

    if (is_bit) {
        prime_count += bit_array_count(data->array);
    } else {
        prime_count += data->remaining;
    }

    pthread_mutex_unlock(&lock);
#ifdef DEBUG
    print(data, "finished");
#endif
    pthread_exit(NULL);
}

void init_data(ThreadData *data, bool use_bit_array, int T, int num_threads)
{
    int lower = 0;
    int upper = sqrt(T);
    int diff = (T - upper) / (num_threads - 1);

    for (int i = 0; i < num_threads; i++) {
        data[i].id = i;
        data[i].num_threads = num_threads;
        data[i].type = use_bit_array ? BIT_ARRAY : BOOL_ARRAY;
        data[i].lower = lower;
        data[i].upper = upper;

        if (i == 0) {
            data[i].remaining = upper - lower - 1;
        } else {
            data[i].remaining = upper - lower;
        }

        if (use_bit_array) {
            data[i].array = bit_array_new(upper - lower);
            bit_array_fill(data[i].array);

            if (i == 0) {
                bit_array_unset(data[i].array, 0);
            }
        } else {
            data[i].A = calloc(upper - lower, sizeof(bool));

            if (i == 0) {
                memset(data[i].A + 1, true, upper - lower - 1);
            } else {
                memset(data[i].A, true, upper - lower);
            }
        }

        lower = upper;
        upper = min(T, upper + diff);
    }
}

void sieve_parallel(bool use_bit_array, unsigned int T, int num_threads)
{
#if RELEASE
    struct timespec time_start, time_end, time_diff;
    timer_start();
#endif
    // Initiate
    pthread_t threads[num_threads];
    ThreadData data[num_threads];
    init_data(data, use_bit_array, T, num_threads);
    pthread_mutex_init(&lock, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads);
    pthread_cond_init(&main_cond, NULL);
    pthread_cond_init(&worker_cond, NULL);

    // Start threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, (i == 0) ? sieve_main : sieve_worker, &data[i]);
    }

    // Block until all threads complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);

        if (use_bit_array) {
            bit_array_free(data[i].array);
        } else {
            free(data[i].A);
        }
    }

#if RELEASE
    timer_stop();
#endif
    printf("Prime count: %d\n", prime_count);
}
