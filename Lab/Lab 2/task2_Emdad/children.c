#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pid_t child1, child2;
int status;
int waitpid(); //tells the parent process to wait for child processes and tells parent process about the status of child processes.

int main(int argc, char *argv[])
{
    child1 = fork(); //child 1 is created

    if (child1 == 0)
    {                                                         //child process
        printf("I am child one, my pid is: %d \n", getpid()); //prints out the pid for child 1
    }

    else if (child1 > 0)
    {
        child2 = fork(); //creates child 2

        if (child2 == -1)
        { //shows error if fork can not create child 2
            perror("fork");
            exit(EXIT_FAILURE);
        }

        else if (child2 == 0)
        {
            printf("I am child two, my pid is: %d \n", getpid()); //prints out the pid for child 1
        }

        else
        {
            waitpid(child1, &status, NULL);                       // complete child1 process and get the status
            waitpid(child2, &status, NULL);                       // complete child2 process and get the status
            printf("I am Parent and my pid is: %d \n", getpid()); //prints out the pid for parent process
            printf("terminated! both childs are executed.\n");
        }
    }

    else
    {
        perror("fork"); //fails if fork can not create child 1
        exit(EXIT_FAILURE);
    }

    return 0;
}