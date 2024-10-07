#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void pattern1(int num_processes) {
    for (int i = 1; i <= num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            FILE *file = fopen("results.txt", "a");
            if (file == NULL) {
                perror("fopen");
                exit(1);
            }
            fprintf(file, "Pattern 1 - Child process %d created by main (PID: %d)\n", i, getpid());
            fclose(file);
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }
    for (int i = 1; i <= num_processes; i++) {
        wait(NULL);
    }
}

void pattern2(int num_processes) {
    for (int i = 1; i <= num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            FILE *file = fopen("results.txt", "a");
            if (file == NULL) {
                perror("fopen");
                exit(1);
            }
            fprintf(file, "Pattern 2 - Child process %d created by process %d (PID: %d)\n", i, getppid(), getpid());
            fclose(file);
            if (i == num_processes) {
                exit(0);
            }
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        } else {
            wait(NULL);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);

    if (num_processes < 1 || num_processes > 256) {
        fprintf(stderr, "Number of processes should be between 1 and 256.\n");
        return 1;
    }

    FILE *file = fopen("results.txt", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    fclose(file);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        pattern1(num_processes);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        pattern2(num_processes);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
