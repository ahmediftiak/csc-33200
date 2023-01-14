#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sem.h"

#define CHILD 0 /* Return value of child process from fork call */
#define TRUE 0
#define FALSE 1

#define NumOfDepositAttempt 5	/* How many times Dad will deposit money */
#define NumOfWithdrawAttempt 20 /* Total number of attempts SON_1 and SON_2 has */

#define DepositAmount 60  /* The amound of money Dad deposits at a time */
#define WithdrawAmount 20 /* The amount of money Son withdraws at a time */
#define initBalance 0

FILE *fp1, *fp2, *fp3, *fp4; /* File Pointers */

int main(int argc, char *argv[])
{
	// process IDs
	int pid1;
	int pid2;
	int pid3;
	int N_Att;
	int bal1, value;
	int flag, flag1;
	int status;
	int N;
	int n; // n - number of trials

	// Semaphores
	int semaphoreOne;
	int semaphoreTwo;

	// Initialize the file balance to be $1000
	fp1 = fopen("balance.txt", "w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);

	// trials
	fp4 = fopen("attempt.txt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);

	// number of trials
	fn = fopen("numberOfAttempts.txt", "w");
	n = 0;
	fprintf(fn, "%d\n", n);
	fclose(fn);

	// semaphore creation errors
	semaphoreOne = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	if (semaphoreOne == -1)
	{
		printf("Semaphore cannot be created\n");
		exit(1);
	}
	semaphoreTwo = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	if (semaphoreTwo == -1)
	{
		printf("Semaphore cannot be created\n");
		exit(1);
	}

	sem_create(semaphoreOne, 1);
	sem_create(semaphoreTwo, 10);

	// Forking to create the first son
	if ((pid1 = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid1 == CHILD)
	{
		// First Child Process. dad tries to do some updates
		N = 5;
		for (int i = 1; i <= N; i++)
		{

			// Entering CS
			P(semaphoreTwo);
			P(semaphoreOne);

			printf("Dad is trying to do update.\n");
			// Number of trials goes up
			fn = fopen("numberOfAttempts.txt", "r+");
			fscanf(fn, "%d", &n);
			fseek(fn, 0L, 0);
			n++;
			fprintf(fn, "%d \n", n);
			fclose(fn);

			fp1 = fopen("balance.txt", "r+");
			fscanf(fp1, "%d", &bal1);
			printf("Dad reads balance = %d \n", bal1);

			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand() % 5);
			fseek(fp1, 0L, 0);
			bal1 += 60;
			printf("Dad writes new balance = %d \n", bal1);
			fprintf(fp1, "%d \n", bal1);
			fclose(fp1);
			printf("Dad is done doing update. \n");
			sleep(rand() % 5);

			// Exiting CS
			V(semaphoreOne);
			V(semaphoreTwo);
		}
	}

	else if (pid1 > 0)
	{
		// Forking another child Create child 2 processs (son #2)
		if ((pid2 = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}
		else if (pid2 == 0)
		{
			printf("First Son's Pid: %d\n", getpid());
			//Second child process. First poor son tries to do updates.
			flag = false;
			while (flag == false)
			{
				// Entering Semaphore
				P(semaphoreTwo);
				P(semaphoreOne);

				// Number of trials goes up
				fn = fopen("numberOfAttempts.txt", "r+");
				fscanf(fn, "%d", &n);
				fseek(fn, 0L, 0);
				n++;
				fprintf(fn, "%d \n", n);
				fclose(fn);

				fp3 = fopen("attempt.txt", "r+");
				fscanf(fp3, "%d", &N_Att);
				if (N_Att == 0)
				{
					fclose(fp3);
					flag = true;
				}
				else
				{
					printf("SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance.txt", "r+");
					fscanf(fp2, "%d", &bal1);
					printf("SON_1 reads balance. Available Balance: %d \n", bal1);

					//clearly there is a error here it was ==
					// COME BACK HERE TO SEE WHATS UP
					if (bal1 < 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand() % 5);
						fseek(fp2, 0L, 0);
						bal1 -= 20;
						printf("SON_1 write new balance: %d \n", bal1);
						fprintf(fp2, "%d\n", bal1);
						fclose(fp2);
						printf("SON_1 done doing update.\n");
						fseek(fp3, 0L, 0);
						N_Att -= 1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}
				// Exiting CS
				V(semaphoreTwo);
				V(semaphoreOne);
			}
		}
		else if (pid2 > 0)
		{
			// Parent Process. Fork off one more child process.
			if ((pid3 = fork()) == -1)
			{
				perror("fork");
				exit(1);
			}
			else if (pid3 == CHILD)
			{
				printf("Second Son's Pid: %d\n", getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = false;
				while (flag1 == false)
				{
					// Entering CS
					P(semaphoreTwo);
					P(semaphoreOne);

					// Number of trials goes up
					fn = fopen("numberOfAttempts.txt", "r+");
					fscanf(fn, "%d", &n);
					fseek(fn, 0L, 0);
					n++;
					fprintf(fn, "%d \n", n);
					fclose(fn);

					fp3 = fopen("attempt.txt", "r+");
					fscanf(fp3, "%d", &N_Att);
					if (N_Att == 0)
					{
						fclose(fp3);
						flag1 = true;
					}
					else
					{
						printf("SON_2 wants to withdraw money\n");
						fp2 = fopen("balance.txt", "r+");
						fscanf(fp2, "%d", &bal1);
						printf("SON_2 reads balance. Available Balance: %d \n", bal1);
						if (bal1 < 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand() % 5);
							fseek(fp2, 0L, 0);
							bal1 -= 20;
							printf("SON_2 write new balance: %d \n", bal1);
							fprintf(fp2, "%d\n", bal1);
							fclose(fp2);

							printf(" SON_2 done doing update\n");
							fseek(fp3, 0L, 0);
							N_Att -= 1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}

					// Exiting CS
					V(semaphoreOne);
					V(semaphoreTwo);
				}
			}
			else if (pid3 > 0)
			{
				//Now parent process waits for the child processes to finish
				waitpid(pid3, &status, 0);
				printf("Process(pid = %d) exited with the status %d. \n", pid3, status);

				waitpid(pid2, &status, 0);
				printf("Process(pid = %d) exited with the status %d. \n", pid2, status);

				waitpid(pid1, &status, 0);
				printf("Process(pid = %d) exited with the status %d. \n", pid1, status);

				// Number of Trials goes up
				fn = fopen("numberOfAttempts.txt", "r+");
				fscanf(fn, "%d", &n);
				printf("Number of attempts: %d \n", n);
				fclose(fn);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
	return 0;
}
