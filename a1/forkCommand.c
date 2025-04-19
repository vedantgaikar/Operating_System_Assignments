#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void copyFunc(char *src, char *dest) {
    pid_t p = fork();
    if (p < 0) {
        printf("[ERROR] Fork failed!\n");
        exit(1);
    } else if (p == 0) {
        printf("[Child Process: %d] Executing copy command...\n", getpid());
        execlp("./copyCommand", "copyCommand", src, dest, NULL);
        perror("Exec failed"); 
        exit(1);
    } else {
        wait(NULL);
        printf("[Parent Process: %d] Copy operation completed!\n", getpid());
    }
}

void grepFunc(char *searchString, char *file) {
    pid_t p = fork();
    if (p < 0) {
        printf("[ERROR] Fork failed!\n");
        exit(1);
    } else if (p == 0) {
        printf("[Child Process: %d] Executing grep command...\n", getpid());
        execlp("./grepCommand", "grepCommand", searchString, file, NULL);
        perror("Exec failed"); 
        exit(1);
    } else {
        wait(NULL);
        printf("[Parent Process: %d] Grep operation completed!\n", getpid());
    }
}

int main() {
    char source[100], dest[100], pattern[100];
    
    printf("Enter source file name: ");
    scanf("%s", source);
    printf("Enter destination file name: ");
    scanf("%s", dest);
    printf("Enter search pattern: ");
    scanf("%s", pattern);

    while (1) {
        int choice;
        printf("\n======= MENU =======\n");
        printf("1. Copy File\n");
        printf("2. Search in File (grep)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                copyFunc(source, dest);
                break;
            case 2:
                grepFunc(pattern, source);
                break;
            case 3:
                printf("[Process ID: %d] Exiting...\n", getpid());
                return 0;
            default:
                printf("[ERROR] Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
