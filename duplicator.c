//
// Created by Kacper Kuchta on 5/15/23.
//
#include "simple_semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>


#define PROGRAM argv[1]
#define PROCESSES argv[2]
#define SECTIONS argv[3]
#define FILE argv[4]
#define SEMAPHORE argv[5]
#define SYNCHRONIZATION argv[6]

char *semName;
bool synchronized;

void cleanUp(void) {
    if (synchronized) unlink_semaphore(semName);
}

void sigHandler(int signal) {
    if (synchronized) {
        printf("SIGINT, unlinking semaphore\n");
        _exit(0);
    }
}

int main(int argc, char *argv[]) {

    /* checking parameters */
    if (argc != 7) {
        perror("Incorrect number of parameters\n");
        printf("To run program correctly type: ./programName program numberOfProcesses numberOfSections textFile semaphoreName synchronization");
        exit(EXIT_FAILURE);
    }

    semName = SEMAPHORE;
    synchronized = (SYNCHRONIZATION[0] == '1') != 0;

    if (atexit(cleanUp) != 0) {
        perror("Atexit error\n");
        exit(EXIT_FAILURE);
    }

    /* signal handle */
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        perror("Signal error\n");
        exit(EXIT_FAILURE);
    }


    /* create fileDescriptor */
    int fileDescriptor = open(FILE, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fileDescriptor == -1) {
        perror("Failed to open fileDescriptor\n");
        exit(EXIT_FAILURE);
    }

    /* write '0' to fileDescriptor */
    if (write(fileDescriptor, "0", sizeof("0")) == -1) {
        perror("Cannot write to fileDescriptor\n");
        exit(EXIT_FAILURE);
    }

    /* close fileDescriptor */
    if (close(fileDescriptor)) {
        perror("Close fileDescriptor error\n");
        exit(EXIT_FAILURE);
    }


    /* create semaphore */
    if (synchronized) {
        int semVal;
        sem_t sem = create_semaphore(SEMAPHORE);
        get_value_semaphore(&sem, &semVal);
        printf("Semaphore's adress: (%d)\nSemaphore's value: %d\n", sem, semVal);
    }




    /* create n-processes */
    int processes = atoi(PROCESSES);
    int statLock = 0;
    for (int i = 0; i < processes; i++) {
        switch (fork()) {
            case -1:
                perror("Fork error\n");
                exit(EXIT_FAILURE);

            case 0:
                if (execlp(PROGRAM, PROGRAM, SECTIONS, FILE, SEMAPHORE, SYNCHRONIZATION,  (char*) NULL) == -1) {
                    printf("%s", PROGRAM);
                    perror("Execlp error\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
        }
    }


    /* waiting for child processes */
    for (int i = 0; i < processes; i++) {
        if (wait(&statLock) == -1) {
            perror("Wait error\n");
            exit(EXIT_FAILURE);
        }
    }


    /* open fileDescriptor again */
    if ((fileDescriptor = open(FILE, O_RDONLY, 0644)) == -1) {
        perror("Open again exit\n");
        exit(EXIT_FAILURE);
    }

    /* read data from fileDescriptor */
    char buffer[10];
    ssize_t bytesRead = read(fileDescriptor, buffer, sizeof(buffer));

    switch (bytesRead) {
        case -1:
            perror("Cannot read data from fileDescriptor\n");
            exit(EXIT_FAILURE);

        case 0:
            perror("File is empty\n");
            exit(EXIT_FAILURE);

            /* checking validity */
        default:
            buffer[bytesRead] = '\0';
            int expected = processes * atoi(SECTIONS);
            int result = atoi(buffer);
            if (result == expected) {
                printf("\x1b[32mProgram exit success, number in fileDescriptor %s is equal to %d\x1b[0m\n", buffer,
                       expected);
            } else {
                printf("\x1b[31mProgram exit failure, number in fileDescriptor %s ,isn't equal to %d\x1b[0m\n", buffer,
                       expected);
            }
            break;
    }

    /* close fileDescriptor */
    if (close(fileDescriptor)) {
        perror("Cannot close fileDescriptor\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}