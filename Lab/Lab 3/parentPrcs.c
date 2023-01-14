/*
Name: Md Shahid Bin Emdad
EMPL: 23726181

Part 1 Write a program where a child is created to execute command that tells you the date
and time inUnix.
Use execl(...).
Note, you need to specify the full path of the file name that gives you date and time
information.Announce the successful forking of child process by displaying its PID.

*/

#include <stdio.h>
#include <unistd.h>

int main()
{
	int child_1 = 0, child_2 = 0;

	child_1 = fork();
	wait();
	if (child_1 < 0)
	{
		perror("fork");
	}

	else if (child_1 == 0)
	{
		char *args[] = {NULL};
		execv("./Prcs_P1", args);
		perror("execv");
	}

	child_2 = fork();
	wait();

	if (child_2 < 0)
	{
		perror("fork");
	}

	else if (child_2 == 0)
	{
		char *args[] = {NULL};
		execv("./Prcs_P2", args);
		perror("execv");
	}

	return 0;
}