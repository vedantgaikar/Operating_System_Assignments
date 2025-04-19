#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <search_string> <source_file>\n", argv[0]);
        return 1;
    }

    char *searchString = argv[1];
    FILE *sourceFile = fopen(argv[2], "r");

    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 1;
    }

    printf("[Process ID: %d] Searching for \"%s\" in %s...\n", getpid(), searchString, argv[2]);

    char line[1024];
    int found = 0;
    
    while (fgets(line, sizeof(line), sourceFile)) {
        if (strstr(line, searchString) != NULL) {
            printf("Match Found: %s", line);
            found = 1;
        }
    }

    fclose(sourceFile);

    if (found) {
        printf("[Process ID: %d] Search completed successfully!\n", getpid());
    } else {
        printf("[Process ID: %d] No matches found.\n", getpid());
    }

    return 0;
}
