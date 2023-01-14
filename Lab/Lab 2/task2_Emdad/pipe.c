#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char *filepath = argv[1]; //saves the filepath
    errno = 0;
    ssize_t numRead1, numRead2;
    int pipefd[2]; //contains both read[0] and write[1] ends
    int waitpid(); //wait for child process and get the status
    int status;

    if (pipe(pipefd) == -1)
    { //creates a pipe otherwise will show error
        perror("PIPE");
    }

    pid_t child = fork(); //create child process
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //child process

        close(pipefd[0]); // close read end

        int file1 = open(filepath, O_RDWR); //checks if the file can be opened and has read and write access
        char buffer[2];
        while ((numRead1 = read(file1, buffer, 1)) > 0)
        { //reads the file and saves it in the buffer

            buffer[numRead1] = '\0';            //null termination character
            write(pipefd[1], buffer, numRead1); //writes the information from the buffer
        }
        close(file1); //close file
    }
    else
    {
        close(pipefd[1]);              //close write end
        waitpid(child, &status, NULL); //wait to child process complete

        int file2 = open(filepath, O_RDWR | O_APPEND); //checks if the file exits and has read, write and append options;
        write(file2, "Parent is writing.\n", 22);      //it writes the given string to the file

        char buffer[2];
        while ((numRead2 = read(pipefd[0], buffer, 1)) > 0)
        {                                   //reads the file and saves it in the buffer
            buffer[numRead2] = '\0';        //null termination character
            write(file2, buffer, numRead2); //writes the information from the buffer
        }
        close(pipefd[0]); //close read end
        close(file2);     //close file
    }

    if (errno == ENOENT)
        printf("%s does not exist\n", filepath); //file exists or not
    else if (errno == EACCES)
        printf("%s is not accessible\n", filepath); //file is accessable or not

    return 0;
}
