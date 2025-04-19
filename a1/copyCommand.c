#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    FILE *sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 1;
    }

    FILE *destFile = fopen(argv[2], "w");
    if (destFile == NULL) {
        perror("Error creating destination file");
        fclose(sourceFile);
        return 1;
    }

    printf("[Process ID: %d] Copying file %s to %s...\n", getpid(), argv[1], argv[2]);

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), sourceFile)) {
        fputs(buffer, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);

    printf("[Process ID: %d] File copied successfully!\n", getpid());

    return 0;
}
