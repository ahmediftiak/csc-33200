
// Name: MdShahid Emdad
// EMPL: 23726181

// There are 10 students enrolled in a course. The course covers x number of
// chapters from a textbook (x > 1).
// In each chapter y number of homeworks are assigned (y>=1). The average grade
// for each homework in all the chapters need to be found out.
// To solve this, write program which has the main process as Director process,
// which reads a file containing grades of all homeworks of all chapters and creates x
// number of Manager processes. Each Manager process will take care of solving
// a chapter. Each manager process will create y number of Worker process and
// pass marks of 10 students to each of them and they calculate and print the average.
// The input file should contain the data according to the value of x and y.

// SOLUTION

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    int child_status = 0;
    //Allocated memory for commands (execvp)
    char memory[30];
    char input_str[20];
    char *token;
    char *arg_arr[30];

    //Wait for input and check it quit is typed.
    scanf("%[^\n]s", input_str);
    int x = strcmp("quit", input_str);
    int args = 0;

    while (x != 0)
    {
        // Tokenize input to split commands and arguments
        token = strtok(input_str, " ");

        while (token != NULL)
        {
            arg_arr[args++] = token;
            token = strtok(NULL, " ");
        }

        // Create Child
        child_status = fork();

        if (child_status < 0)
        {
            perror("fork");
        }

        else if (child_status == 0)
        {
            //Execute the command.
            if (execvp(*arg_arr, arg_arr) < 0)
            {
                perror("execvp");
                return 1;
            }
        }

        else
        {
            wait(&child_status);
            execvp(argv[0], argv);
        }
    }

    return 0;
}