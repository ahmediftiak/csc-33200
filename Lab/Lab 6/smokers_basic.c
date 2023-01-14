#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>

#include "sem.h"

#define N 2
#define BUFSIZE 1
#define PERMS 0666 //0666 - To grant read and write permissions

#define TOBACCO 0
#define PAPER 1
#define MATCH 2

int *buffer;
int nextp=0;
int mutex,full,empty,tobacco,paper,match;    /* semaphore variables
						  * mutex - binary semaphore -- critical section
 						  *	full, empty - counting semaphore
              * queue - semaphore for curr_smokers*/

int main()
{
 int shmid,no=1,i;
 int pid,n;

 if((shmid=shmget(1000,BUFSIZE,IPC_CREAT | PERMS)) < 0)
 {
  fprintf(stderr,"\n unable to create shared memory");
  return 1;
 }
 if((buffer=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
 {
  fprintf(stderr,"\n Shared memory allocation error\n");
  exit(1);
 }

 // semaphore creation
 if((mutex=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
   fprintf(stderr,"\n can't create mutex semaphore");
   exit(1);
 }

 if((empty=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\n can't create empty semaphore");
  exit(1);
 }

 if((full=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\ncan't create full semaphore");
  exit(1);
 }

 if((tobacco=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\ncan't create full semaphore");
  exit(1);
 }

 if((paper=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\ncan't create full semaphore");
  exit(1);
 }

 if((match=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\ncan't create full semaphore");
  exit(1);
 }

 // initialze the semaphore
 sem_create(mutex,1);
 sem_create(empty,N);
 sem_create(full,0);
 sem_create(tobacco,0);
 sem_create(paper,0);
 sem_create(match,0);


  int smoker_tobacco, smoker_paper, smoker_match = -1;

  fprintf(stderr,"Forking smokers (three smokers)\n");
  smoker_tobacco = fork();
  if (smoker_tobacco > 0){

    smoker_paper = fork();
    if (smoker_paper > 0){

      smoker_match = fork();
    }
  }

  //fprintf(stderr,"%d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);

  //Agent process
  if(smoker_tobacco && smoker_paper && smoker_match){
    //fprintf(stderr,"Agent pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    int i = 10;
    printf("Starting procedure (set to perform %d rounds)\n",i);
    while(i){
      i--;
      int randnum = rand() % 3;
  	  P(empty);
  	  P(mutex); // Entering critical section
      fprintf(stderr,"%d: Agent putting things on the table\n", i);
  	  for(int i = 0; i < 3; i++){
        if(i!=randnum){
          *(buffer+(nextp++)) = i;
          nextp = nextp % 2;
          if (i == TOBACCO){
            fprintf(stderr,"\tAgent put tobacco (item 0) on the table\n");
          } else if (i == PAPER){
            fprintf(stderr,"\tAgent put paper (item 1) on the table\n");
          } else {
            fprintf(stderr,"\tAgent put match (item 2) on the table\n");
          }
        }
      }
  	  V(mutex); // Exit from critical section
      if (i == TOBACCO){
        V(tobacco);
      } else if (i == PAPER){
        V(paper);
      } else {
        V(match);
      }
  	  V(full);
    }
  }

  //Smoker process
  if(smoker_tobacco == 0){
    //fprintf(stderr,"Smoker with tobacco pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      P(tobacco);
      fprintf(stderr,"Smoker with tobacco called by agent\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with tobacco found items %d and %d on table\n",*(buffer),*(buffer+1));
      fprintf(stderr,"\tSmoker with tobacco has smoked\n");
      V(mutex);
      V(tobacco);
      V(empty);
    }
  }

  if(smoker_paper == 0){
    //fprintf(stderr,"Smoker with paper pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      P(paper);
      fprintf(stderr,"Smoker with paper called by agent\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with paper found items %d and %d on table\n",*(buffer),*(buffer+1));
      fprintf(stderr,"\tSmoker with paper has smoked\n");
      V(mutex);
      V(empty);
    }
  }

  if(smoker_match == 0){
    //fprintf(stderr,"Smoker with match pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      P(match);
      fprintf(stderr,"Smoker with match called by agent\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with match found items %d and %d on table\n",*(buffer),*(buffer+1));
      fprintf(stderr,"\tSmoker with match has smoked\n");
      V(mutex);
      V(empty);
    }
  }

  return 0;
}
