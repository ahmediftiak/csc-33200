/*
Name: Md Shahid Bin Emdad
EMPL: 23726181

Prcs_P2.c: Copy the contents of source.txt1 into destination1.txtand
destination2.txtasper the following procedure.
1. Read the next 100 characters from source.txt, and among characters read, replace
each char-acter ’1’ with character ’L’ and all characters are then written in
destination1.txt
2. Then the next 50 characters are read from source.txt, and among characters read,
replace each character ’3’ with character ’E’ and all characters are then written in
destination2.txt.
3. The previous steps are repeated until the end of file source.txt. The last read may
not have100 or 50 characters
*/

#include <stdio.h>
#include <errno.h>     //errno note
#include <string.h>    //give strerror
#include <sys/types.h> //open file
#include <sys/stat.h>  //open file
#include <fcntl.h>     //open file
#include <unistd.h>    //read and close file

int main(int argc, char const *argv[])
{
    int fd_source, fd_destination_1, fd_destination_2;

    char *SOURCE_PATH = "source.txt";
    char *DestinationPath1 = "destination1.txt";
    char *DestinationPath2 = "destination2.txt";
    size_t BUFFER_SIZE_1 = 100;
    size_t BUFFER_SIZE_2 = 50;
    size_t MAX_BUFFER = BUFFER_SIZE_1;
    size_t MIN_BUFFER = BUFFER_SIZE_2;

    //Check if the source file has read access
    errno = 0;
    fd_source = open(SOURCE_PATH, O_RDONLY, 0600);
    if (fd_source == -1)
    {
        printf("\nERROR : [%s][%s]\n", SOURCE_PATH, strerror(errno));
        return 1;
    }

    // Check if destination1 file has write access
    errno = 0;
    fd_destination_1 = open(DestinationPath1, O_WRONLY, 0600);
    if (fd_destination_1 == -1)
    {
        printf("\nERROR : [%s][%s]\n", DestinationPath1, strerror(errno));
        return 1;
    }

    // Check if destination1 file has write access
    errno = 0;
    fd_destination_2 = open(DestinationPath2, O_WRONLY, 0600);
    if (fd_destination_2 == -1)
    {
        printf("\nERROR : [%s][%s]\n", DestinationPath2, strerror(errno));
        return 1;
    }

    // Read, replace and store info into the destination files
    int b, fr1, fr2, counter = 0;
    char buffer[MAX_BUFFER];
    do
    {
        counter++;
        if (counter % 2 == 1)
        //odd counters are going into destination1
        {
            fr1 = read(fd_source, buffer, BUFFER_SIZE_1);
            b = fr1;
            //loop through the buffer
            for (int i = 0; i < fr1; i++)
            {
                if (buffer[i] == '1')
                    buffer[i] = 'A'; //replace every '1' with a 'A'
            }
            write(fd_destination_1, buffer, fr1);
        }
        else
        //even coutners are going into destination2
        {
            fr2 = read(fd_source, buffer, BUFFER_SIZE_2); //store the count bytes into fr
            b = fr2;
            //loop through the buffer
            for (int i = 0; i < fr2; i++)
            {
                if (buffer[i] == '2')
                    buffer[i] = 'B'; //replace every '1' with a 'B'
            }
            write(fd_destination_2, buffer, fr2);
        }

    } while (b >= MIN_BUFFER);

    return 0;
}