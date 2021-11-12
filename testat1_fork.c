#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int process;
    int NUMBER_OF_PROCESSES = 3;

    for (process = 1; process <= NUMBER_OF_PROCESSES; process++)
    {
        switch (fork())
        {
        case -1:
            perror("FORK failed");
            exit(1);
        case 0: // Child
            printf("Child process %d (%d) started\n", process, getpid());

            for (int i = 0; i < 3; i++) {
                printf("Prozess %d betritt kritischen Bereich\n", process);
                sleep(1);
                printf("Prozess %d verlässt kritischen Bereich\n", process);
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