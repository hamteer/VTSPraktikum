// include standard library functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// include functions for semaphores
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

// define sem_nums as talking variables
#define MUTEX 0
#define WRITER 1
#define READER 2


int sem_num;
int sem_id;



void init_sem() {
    union semnum {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = 1;

    // set MUTEX  to 1
    semctl(sem_id, MUTEX, SETVAL, 1);

    // set WRITER to 1
    semctl(sem_id, WRITER, SETVAL, 1);

    // set READER to 0
    semctl(sem_id, READER, SETVAL, 0);
} 

void P(int sem_num) {
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;      // P
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, &semaphore, 1)) {
        perror("Error in semop P()");
        exit(1);
    }
}

void V(int sem_num) {
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1;      // V
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if (semop(sem_id, &semaphore, 1)) {
        perror("Error in semop V()");
        exit(1);
    }
} 

int main() {
    int process;
    int NUMBER_OF_PROCESSES = 7;
    int sem_key;
    
    // Create unique key and test if ftok worked
    if ((sem_key = ftok("/home/lars/Temp/TestVisualStudioCode-C/VTSPraktikum/testat1", '1')) < 0) {
        perror("Error in ftok");
        exit(0);
    }

    // Create semaphore group and test if it worked
    if ((sem_id = semget(sem_key, 3, IPC_CREAT|0666)) < 0) {
        perror("Error in semget");
        exit(0);
    }  

    init_sem(); 

    int reader_number = 0;

    for (process = 1; process <= NUMBER_OF_PROCESSES; process++) {
        switch (fork()) {
        case -1:
            perror("FORK failed");
            exit(1);
        case 0: // Child
            printf("Child process %d (%d) started\n", process, getpid());

            if (process % 3 == 0) {     // writer
                for (int i = 0; i < 3; i++) {
                    P(WRITER);
                    printf("Prozess %d (Writer): Schreibzugriff auf Variable (kritisch) starten\n", process);
                    sleep(1);
                    V(WRITER);
                    printf("Prozess %d (Writer): Schreibzugriff auf Variable (kritisch) beendet\n", process);
                    printf("Prozess %d (Writer) betritt unkritischen Bereich\n", process);
                    sleep(1);
                    printf("Prozess %d (Writer) verlässt unkritischen Bereich\n", process);
                }
            } else {        // Writer
                for (int i = 0; i < 3; i++) {
                    P(MUTEX);
                    reader_number = semctl(sem_id, READER, GETVAL);
                    reader_number++;
                    semctl(sem_id, READER, SETVAL, reader_number);
                    if (semctl(sem_id, READER, GETVAL) == 1) {
                        P(WRITER);
                    }
                    V(MUTEX);
                    printf("Prozess %d (Reader): Lesezugriff auf Variable (kritisch) starten\n", process);
                    sleep(1);
                    P(MUTEX);
                    reader_number = semctl(sem_id, READER, GETVAL);
                    reader_number--;
                    semctl(sem_id, READER, SETVAL, reader_number);
                    if (semctl(sem_id, READER, GETVAL) == 0) {
                        V(WRITER);
                    } 
                    V(MUTEX);
                    printf("Prozess %d (Reader): Lesezugriff auf Variable (kritisch) beendet\n", process);
                    printf("Prozess %d (Reader) betritt unkritischen Bereich\n", process);
                    sleep(1);
                    printf("Prozess %d (Reader) verlässt unkritischen Bereich\n", process);
                }    
            }              
 
            printf("Prozess %d beendet\n", process);

            exit(0);

        default: // Father
            break;
        }
    }

    // printf("Father process (%d) stopped\n", getpid());
    return 0;
}