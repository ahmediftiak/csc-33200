#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    //parent P
    int a = 10, b = 25, fq = 0, fr = 0;
    fq = fork(); // fork a child - call it Process Q

    if (fq == 0)
    { // Child successfully forked
        a = a + b;
        printf("value of a: %d \n", a);        //prints the value of a
        printf("value of b: %d \n", b);        //prints the value of b
        printf("process id: %d \n", getpid()); //prints the process id of Q

        fr = fork(); // fork another child - call it Process R
        if (fr != 0)
        {
            b = b + 20;
            printf("value of a: %d \n", a);        //prints the value of a
            printf("value of b: %d \n", b);        //prints the value of b
            printf("process id: %d \n", getpid()); //prints the process id for R
        }

        else
        {
            a = (a * b) + 30;
            printf("value of a: %d \n", a);        //prints the value of a
            printf("value of b: %d \n", b);        //prints the value of b
            printf("process id: %d \n", getpid()); //prints the process id
        }
    }

    else
    {
        b = a + b - 5;
        printf("value of a: %d \n", a);        //prints the value of a
        printf("value of b: %d \n", b);        //prints the value of b
        printf("process id: %d \n", getpid()); //prints the process id
    }

    return 0;
}