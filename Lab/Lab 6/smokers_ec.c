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

#define SIGCONT 18

int *buffer;
int nextp=0;
int mutex,full,empty,queue;    /* semaphore variables
						  * mutex - binary semaphore -- critical section
 						  *	full, empty - counting semaphore
              * queue - semaphore for curr_smokers*/
int *curr_smoker;
int *smoker_queue;

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

 if((curr_smoker=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
 {
  fprintf(stderr,"\n Shared memory allocation error\n");
  exit(1);
 }

 if((smoker_queue=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
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

 if((queue=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
 {
  fprintf(stderr,"\ncan't create full semaphore");
  exit(1);
 }

 // initialze the semaphore
 sem_create(mutex,1);
 sem_create(empty,N);
 sem_create(full,0);
 sem_create(queue,1);


  *(curr_smoker) = 1;
  int smoker_tobacco, smoker_paper, smoker_match = -1;
  //char items[3] = {"Tobacco", "Paper", "Match"};

  fprintf(stderr,"Forking smokers (three smokers)\n");
  smoker_tobacco = fork();
  if (smoker_tobacco > 0){

    smoker_paper = fork();
    if (smoker_paper > 0){

      smoker_match = fork();
      if (smoker_match > 0){
        *(smoker_queue) = smoker_tobacco;
        *(smoker_queue+1) = smoker_paper;
        *(smoker_queue+2) = smoker_match;
        fprintf(stderr,"Queue created: %d,%d,%d\n",*(smoker_queue+0),*(smoker_queue+1),*(smoker_queue+2));
      }
    }
  }

  //fprintf(stderr,"%d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);

  //Agent process
  if(smoker_tobacco && smoker_paper && smoker_match){
    //fprintf(stderr,"Agent pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      int randnum = rand() % 3;
  	  P(empty);
  	  P(mutex); // Entering critical section
      fprintf(stderr,"Agent putting things on the table\n");
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
  	  V(full);

      P(queue);
      fprintf(stderr,"\tAgent waking up next smoker in queue: %d\n", *curr_smoker);
      kill(*(smoker_queue+*curr_smoker), SIGCONT); //
      V(queue);
    }
  }

  //Smoker process
  if(smoker_tobacco == 0){
    //fprintf(stderr,"Smoker with tobacco pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      //fprintf(stderr,"Smoker with tobacco going to sleep\n");
      sleep();
      fprintf(stderr,"Smoker with tobacco has awaken with item 0\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with tobacco found items %d and %d on table\n",*(buffer),*(buffer+1));
      if (*(buffer) != 0 && *(buffer+1) != 0){
        fprintf(stderr,"\tSmoker with tobacco has smoked\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        V(queue);
        V(empty);
      }else{
        fprintf(stderr,"\tSmoker with tobacco was not able to smoke\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        fprintf(stderr,"\tSmoker with tobacco waking up next smoker in queue: %d\n", *curr_smoker);
        kill(*(smoker_queue+*curr_smoker), SIGCONT);
        V(queue);
        V(full);
      }
      V(mutex);
    }
  }

  if(smoker_paper == 0){
    //fprintf(stderr,"Smoker with paper pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      //fprintf(stderr,"Smoker with paper going to sleep\n");
      sleep();
      fprintf(stderr,"Smoker with paper has awaken with item 1\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with paper found items %d and %d on table\n",*(buffer),*(buffer+1));
      if (*(buffer) != 1 && *(buffer+1) != 1){
        fprintf(stderr,"\tSmoker with paper has smoked\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        V(queue);
        V(empty);
      }else{
        fprintf(stderr,"\tSmoker with paper was not able to smoke\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        fprintf(stderr,"\tSmoker with paper waking up next smoker in queue: %d\n", *curr_smoker);
        kill(*(smoker_queue+*curr_smoker), SIGCONT);
        V(queue);
        V(full);
      }
      V(mutex);
    }
  }

  if(smoker_match == 0){
    //fprintf(stderr,"Smoker with match pids: %d,%d,%d\n",smoker_tobacco,smoker_paper,smoker_match);
    while(1){
      //fprintf(stderr,"Smoker with match going to sleep\n");
      sleep();
      fprintf(stderr,"Smoker with match has awaken with item 2\n");
      P(full);
      P(mutex);
      fprintf(stderr,"\tSmoker with match found items %d and %d on table\n",*(buffer),*(buffer+1));
      if (*(buffer) != 2 && *(buffer+1) != 2){
        fprintf(stderr,"\tSmoker with match has smoked\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        V(queue);
        V(empty);
      }else{
        fprintf(stderr,"\tSmoker with match was not able to smoke\n");
        P(queue);
        *curr_smoker += 1;
        *curr_smoker = *curr_smoker % 3;
        fprintf(stderr,"\tSmoker with match waking up next smoker in queue: %d\n", *curr_smoker);
        kill(*(smoker_queue+*curr_smoker), SIGCONT);
        V(queue);
        V(full);
      }
      V(mutex);
    }
  }

  return 0;
}
