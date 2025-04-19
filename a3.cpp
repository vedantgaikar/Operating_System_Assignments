#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
int main()
{
    int fd[2];
    pid_t pid;
    int filedes = open("new_file.txt", O_RDONLY);

    if (filedes == -1)
    {
        perror("Error opening file");
        exit(1);
    }

    dup2(filedes, fileno(stdin));
    close(filedes);

    pipe(fd);

    pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], fileno(stdout));
        close(fd[0]);
        close(fd[1]);
        printf("Sort process exec");
        execl("/usr/bin/sort", "sort", NULL);
    }
    else if (pid > 0)
    {
        // Parent process
        dup2(fd[0], fileno(stdin));
        close(fd[1]);
        close(fd[0]);
        printf("Uniq process exec");
        execl("/usr/bin/uniq", "uniq", "-c", NULL);
    }
    else
    {
        perror("fork");
        exit(1);
    }

    return 0;
}
