//
// Created by Kacper Kuchta on 5/15/23.
//
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

sem_t *create_semaphore(const char *name) {
    sem_t *sem = sem_open(name, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("Failed to create semaphore\n");
        exit(EXIT_FAILURE);
    }

    return sem;
}

sem_t *open_semaphore(const char *name) {
    sem_t *sem = sem_open(name, O_RDONLY);
    if (sem == SEM_FAILED) {
        perror("Cannot to open semaphore\n");
        exit(EXIT_FAILURE);
    }

    return sem;
}

void close_semaphore(sem_t *sem) {
    if (sem_close(sem) == -1) {
        perror("Semaphore close error\n");
        exit(EXIT_FAILURE);
    }
}

void unlink_semaphore(const char *name) {
    if (sem_unlink(name) == -1) {
        perror("Unlink error\n");
        exit(EXIT_FAILURE);
    }
}

void post_semaphore(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Failed to post semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void wait_semaphore(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Semaphore wait error\n");
        exit(EXIT_FAILURE);
    }
}

void get_value_semaphore(sem_t *sem, int *sval) {
    if (sem_getvalue(sem, sval) == -1) {
        perror("Semaphore get value error\n");
        exit(EXIT_FAILURE);
    }
}