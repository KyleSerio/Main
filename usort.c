/************************************************************************
 * 
 * UCSC Operating Systems Assignment 1
 * 
 * UNIX Shared Memory Multi-Process Merge Sort
 * 
 * Original assignment written 2020-2021 David C. Harrison.
 * This solution written by Kyle Serio.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

#include "merge.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* LEFT index and RIGHT index of the sub-array of ARR[] to be sorted */
void singleProcessMergeSort(int arr[], int left, int right) 
{
  if (left < right) {
    int middle = (left+right)/2;
    singleProcessMergeSort(arr, left, middle); 
    singleProcessMergeSort(arr, middle+1, right); 
    merge(arr, left, middle, right); 
  } 
}

void multiProcessMergeSort(int arr[], int left, int right) 
{

  int shmid = shmget(IPC_PRIVATE, sizeof(int)*(right+1), 0666|IPC_CREAT); //Create Shared Memory
  int *shm = shmat(shmid,(void*)0,0); //Attach Shared Memory

  
  int middle = (left+right)/2;
  for(int index = 0; index <= middle; index++) //Copy Left side into shared memory
  {
    shm[index] = arr[index];
  }

  switch (fork())
  {
    case -1:
      printf("Error - Exit");
      exit(-1);
    case 0:
      shm = shmat(shmid,(void*)0,0); //Attach Child to Shared Memory
      singleProcessMergeSort(shm,left,middle); //Use Child Labor to Sort Left Half
      shmdt(shm); //Kick Child Out
      exit(0); //Break His Things
    default:
      singleProcessMergeSort(arr,middle+1,right);//Papa Sorts The Right Side
      wait(NULL); //Make Sure Child Finished His Chores

      for(int index = 0; index <= middle; index++)//Copy Child's Work Back to Local
      {
        arr[index] = shm[index];
      }

      shmdt(shm); //Hang Up The Hat
      shmctl(shmid,IPC_RMID,NULL);//Push the Hat Rack Over
      merge(arr,left,middle,right);//Fusion!
  }
      
}
