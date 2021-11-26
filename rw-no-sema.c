#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int process;
    int NUMBER_OF_PROCESSES = 7;

    for (process = 1; process <= NUMBER_OF_PROCESSES; process++)
    {
        switch (fork())
        {
        case -1:
            perror("FORK failed");
            exit(1);
        case 0: // Child
            printf("Child process %d (%d) started\n", process, getpid());

            if (process % 3 == 0) {     // writer
                for (int i = 0; i < 3; i++) {
                    printf("Prozess %d (Writer): Schreibzugriff auf Variable (kritisch) starten\n", process);
                    sleep(1);
                    printf("Prozess %d (Writer): Schreibzugriff auf Variable (kritisch) beendet\n", process);
                    printf("Prozess %d (Writer) betritt unkritischen Bereich\n", process);
                    sleep(1);
                    printf("Prozess %d (Writer) verlässt unkritischen Bereich\n", process);
                }
            } else {                    // reader
                for (int i = 0; i < 3; i++) {
                    printf("Prozess %d (Reader): Lesezugriff auf Variable (kritisch) starten\n", process);
                    sleep(1);
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

    printf("Father process (%d) stopped\n", getpid());
    return 0;
}