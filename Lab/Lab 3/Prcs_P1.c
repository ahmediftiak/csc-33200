/*
Name: Md Shahid Bin Emdad
EMPL: 23726181

Prcs_P1.c: Create two files namely, destination1.txt and destination2.txt with
read, write,and execute permissions.

*/

#include <stdio.h>
#include <errno.h>     //errno note
#include <string.h>    //give strerror
#include <sys/types.h> //open file
#include <sys/stat.h>  //open file
#include <fcntl.h>     //open file

int main(int argc, char const *argv[])
{
    int fd;
    char *Destination1_PATH = "destination1.txt";
    char *Destination2_PATH = "destination2.txt";

    errno = 0;
    fd = open(Destination1_PATH, O_RDWR | O_CREAT, 0600);
    if (fd == -1)
    {
        printf("\nERROR : [%s][%s]\n", Destination1_PATH, strerror(errno)); //create file or give error
    }

    errno = 0;
    fd = open(Destination2_PATH, O_RDWR | O_CREAT, 0600);
    if (fd == -1)
    {
        printf("\nERROR : [%s][%s]\n", Destination2_PATH, strerror(errno)); //create file or give error
    }

    return 0;
}