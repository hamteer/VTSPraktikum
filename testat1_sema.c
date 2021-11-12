
// include standard library functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// include functions for semaphores
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

int sem_key;
int sem_num;
int sem_id;

void init_sem() {
    union semnum {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = 1;

    if(semctl(sem_id, sem_num, SETVAL, argument) < 0) {
        perror("Error in semctl");
        exit(1);
    }
} 

void P(int sem_num) {
    struct sembuf semaphore;

    semaphore.sem_num = 0;
    semaphore.sem_op = -1;      // P
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, &semaphore, 1)) {
        perror("Error in semop P()");
        exit(1);
    }
}

void V(int sem_num) {
    struct sembuf semaphore;

    semaphore.sem_num = 0;
    semaphore.sem_op = 1;      // P
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, &semaphore, 1)) {
        perror("Error in semop V()");
        exit(1);
    }
} 

int main() {
    int process;
    int NUMBER_OF_PROCESSES = 3;
    
    // Create unique key and test if ftok worked
    if ((sem_key = ftok("/home/lars/Temp/TestVisualStudioCode-C/VTSPraktikum/testat1", '1')) < 0) {
        perror("Error in ftok");
        exit(0);
    }

    // Create semaphore group and test if it worked
    if ((sem_id = semget(sem_key, 1, IPC_CREAT|0666)) < 0) {
        perror("Error in semget");
        exit(0);
    }  

    init_sem(); 

    for (process = 1; process <= NUMBER_OF_PROCESSES; process++) {
        switch (fork()) {
        case -1:
            perror("FORK failed");
            exit(1);
        case 0: // Child
            printf("Child process %d (%d) started\n", process, getpid());

            for (int i = 0; i < 3; i++) {
                P(1);
                printf("Prozess %d betritt kritischen Bereich\n", process);
                sleep(1);
                printf("Prozess %d verlässt kritischen Bereich\n", process);
                V(1);
                printf("Prozess %d betritt unkritischen Bereich\n", process);
                sleep(1);
                printf("Prozess %d verlässt unkritischen Bereich\n", process);
            }

            exit(0);

        default: // Father
            break;
        }
    }

    printf("Father process (%d) stopped\n", getpid());
    return 0;
}