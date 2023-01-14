#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pid_t child1, child2; //processes 2 child
int status;           //checks the status of the child process to compute
int waitpid();
int sum1 = 0, sum2 = 0, sum3 = 0; // all 3 sums

int main(int argc, char *argv[])
{
    int num = atoi(argv[1]); //converts characters -> integer

    if (argc != 2)
    {
        printf("Usage!"); //checks error
        exit(EXIT_FAILURE);
    }

    child1 = fork(); //create child 1

    if (child1 == 0)
    { // process child

        printf("The previous sum of child 1 (for odd nums): "); //prints out the intermediate summation of odd numbers
        int i = 1;
        while (i <= num)
        {
            sum1 += i; //add all odd nums
            i = i + 2;
            printf("%d   ", sum1); //prints out the intermediate summation of odd numbers
        }
        printf("\n");

        printf("sum of child 1 process: %d \n", sum1); //prints out the sum of all odd numbers
    }

    else if (child1 > 0)
    {
        child2 = fork(); //create child 1

        if (child2 == -1)
        {
            perror("fork"); //checks if child2 is created by fork
            exit(EXIT_FAILURE);
        }

        else if (child2 == 0)
        { //child process

            printf("The previous sum of child 2 (for even nums): "); //prints out the intermediate summation of odd numbers
            int l = 2;
            while (l <= num)
            {
                sum2 += l; //add all even nums
                l = l + 2;
                printf("%d   ", sum2); //prints out the intermediate summation of even numbers
            }

            printf("\n");
            printf("sum of child 2 process: %d \n", sum2); //prints out the sum of all even numbers
        }

        else
        {
            waitpid(child1, &status, NULL); // complete child1 process and get the status
            waitpid(child2, &status, NULL); // complete child2 process and get the status

            printf("intermediate sum of parent: ");
            int k = 1;
            while (k <= num)
            {
                sum3 += k;
                k = k + 1;
                printf("%d   ", sum3);
            }

            printf("\n");
            printf("The sum of parent process is: %d \n", sum3); //prints out the sum of all number from 1 to num
        }
    }

    else
    {
        perror("fork"); //checks if child process is created by fork
        exit(EXIT_FAILURE);
    }

    return 0;
}