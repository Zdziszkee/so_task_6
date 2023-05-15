//
// Created by Kacper Kuchta on 5/15/23.
//
#include <semaphore.h>

#ifndef SO_TASK_6_SIMPLE_SEMAPHORE_H
#define SO_TASK_6_SIMPLE_SEMAPHORE_H
sem_t create_semaphore(const char *name);

sem_t open_semaphore(const char *name);

void close_semaphore(sem_t *sem);

void unlink_semaphore(const char *name);

void post_semaphore(sem_t *sem);

void wait_semaphore(sem_t *sem);

void get_value_semaphore(sem_t *sem, int *sval);
#endif //SO_TASK_6_SIMPLE_SEMAPHORE_H
