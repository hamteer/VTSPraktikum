// include standard library functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

// include functions for semaphores
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>


int sem_num;
int sem_id;

void init_sem() {
    union semnum {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = 1;

    for (int x = 0; x < 5; x++) {
        if(semctl(sem_id, x, SETVAL, argument) < 0) {
            perror("Error in semctl");
            exit(1);
        }
    }  
} 

void P(int sem_num1, int sem_num2) {
    struct sembuf semaphore[2];

    semaphore[0].sem_num = sem_num1;
    semaphore[0].sem_op = -1;      // P
    semaphore[0].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    semaphore[1].sem_num = sem_num2;
    semaphore[1].sem_op = -1;      // P
    semaphore[1].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, semaphore, 2)) {
        perror("Error in semop P()");
        exit(1);
    }
}

void V(int sem_num1, int sem_num2) {
    struct sembuf semaphore[2];

    semaphore[0].sem_num = sem_num1;
    semaphore[0].sem_op = 1;      // P
    semaphore[0].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    semaphore[1].sem_num = sem_num2;
    semaphore[1].sem_op = 1;      // P
    semaphore[1].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, semaphore, 2)) {
        perror("Error in semop P()");
        exit(1);
    }
} 


int main() {
    int process;
    int NUMBER_OF_PROCESSES = 5;
    int sem_key;
    time_t t;
    
    // Create unique key and test if ftok worked
    if ((sem_key = ftok("/home/lars/Temp/TestVisualStudioCode-C/VTSPraktikum/testat1", '1')) < 0) {
        perror("Error in ftok");
        exit(0);
    }

    // Create semaphore group and test if it worked
    if ((sem_id = semget(sem_key, 5, IPC_CREAT|0666)) < 0) {
        perror("Error in semget");
        exit(0);
    } 
    
    init_sem(); 

    for (process = 0; process < NUMBER_OF_PROCESSES; process++) {
        switch (fork()) {
        case -1:
            perror("FORK failed");
            exit(1);
        case 0: // Child
            printf("Philosoph p%d setzt sich an den Tisch\n", process);

            for (int j = 0; j < 3; j++) {
                /* Intializes random number generator */
                srand(getpid());

                printf("Philosoph p%d denkt nach...\n", process);
                sleep((rand() % 10) + 1);
                printf("Philosoph p%d ist fertig mit nachdenken und will essen\n", process);
                
                if(process == 0) {
                    P(4,0);
                } else {
                    P(process-1, process);
                }

                printf("Philosoph p%d beginnt zu Essen\n", process);
                sleep((rand() % 10) + 1);
                printf("Philosoph p%d ist fertig mit Essen\n", process);

                if(process == 0) {
                    V(4,0);
                } else {
                    V(process-1, process);
                }
            }
            printf("Philosoph p%d hat aufgegessen, genug nachgedacht und geht.\n", process);
            exit(0);

        default: // Father
            break;
        }
    }

    //printf("Father process (%d) stopped\n", getpid());
    return 0;
}