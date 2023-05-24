#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "simple_semaphore.h"
#include <stdbool.h>


#define SECTIONS argv[1]
#define FILE argv[2]
#define SEMAPHORE argv[3]
#define SYNCHRONIZATION argv[4]

int main(int argc, char **argv) {

    /* checking parameters */
    if (argc != 5) {
        printf("Wrong number of parameters! \n");
        printf("To run program correctly type: ./programName numberOfSections textFile semaphoreName synchronization");
        exit(EXIT_FAILURE);
    }

    /* if synchronization on */
    bool synchronization = (SYNCHRONIZATION[0] == '1') != 0;

    /* open semaphore */
    sem_t *sem = NULL;
    if (synchronization) {
        *sem = open_semaphore(SEMAPHORE);
    }

    /* get number of critical sections */
    int sections = atoi(SECTIONS);

    for (int i = 0; i < sections; i++) {
        /* wait random amount of time */
        srand(time(NULL));
        sleep(rand() % 5);

        int value;
        if (synchronization) {
            get_value_semaphore(sem, &value);
            wait_semaphore(sem);
            printf("PID = %d, semaphore's value before critical section = %d\n", getpid(), value);
        }

        /* critical section */
        int file = open(FILE, O_RDONLY);
        if (file == -1) {
            perror("Failed to open file to bits_read only\n");
            exit(EXIT_FAILURE);
        }

        char buffer[20];
        ssize_t bits_read = read(file, buffer, sizeof(buffer));

        bool fileEnd = false;


        switch (bits_read) {
            case -1:
                perror("Failed to bits_read file\n");
                exit(EXIT_FAILURE);

            case 0:
                fileEnd = true;
                break;

            default:
                buffer[bits_read] = '\0';
                if (synchronization) {
                    get_value_semaphore(sem, &value);
                    printf("PID = %d, semaphore's value = %d, bits_read nr from file = %s, section nr = %d\n", getpid(),
                           value, buffer, i);
                }
        }

        if (fileEnd) break;    //leave for loop

        /* close file */
        if (close(file)) {
            perror("Cannot close file\n");
            exit(EXIT_FAILURE);
        }

        /* increase number by one */
        int number = atoi(buffer);
        number++;

        /* write increased number to buffer */
        sprintf(buffer, "%d", number);

        /* wait random amount of time again */
        sleep(rand() % 5);

        /* open file to write */
        file = open(FILE, O_TRUNC | O_WRONLY | O_CREAT, 0644);
        if ((file) == -1) {
            printf("Failed to open file to write data\n");
            exit(EXIT_FAILURE);
        }

        /* write data to file */
        if (write(file, buffer, strlen(buffer)) == -1) {
            perror("Write to file error\n");
            exit(EXIT_FAILURE);
        }

        /* close file */
        if (close(file)) {
            perror("Cannot close file\n");
            exit(EXIT_FAILURE);
        }


        if (synchronization) {
            post_semaphore(sem);
            get_value_semaphore(sem, &value);
            printf("PID = %d, semaphore's value = %d\n", getpid(), value);
        }
    }

    /* close semaphore */
    if (synchronization) {
        close_semaphore(sem);
    }

    return 0;
}
