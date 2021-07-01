/************************************************************************
 * 
 * UCSC Operating Systems Assignment 1
 * 
 * POSIX Shared Memory Multi-Process Merge Sort
 * 
 * Original assignment written 2020-2021 David C. Harrison.
 * This solution written by Kyle Serio.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ************************************************************************/

#include "merge.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef PSORT_C
#define PSORT_C
#define NAME "test"
#define SIZE sizeof(int)*(right+1)
#endif

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

  int shmid;
  shmid = shm_open(NAME, O_CREAT | O_RDWR, 0666);
  if(shmid < 0)
  {
    printf("Oopsy Daisies!");
  }
 
  ftruncate(shmid, SIZE); //Sets Size
  int* shm = mmap(0,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,shmid,0);

  
  int middle = (left+right)/2;

  for(int index = 0; index <= middle; index++) //Copy Left side into shared memory
  {
    shm[index] = arr[index];
  }


  switch (fork())
  {
    case-1:
      printf("Error - Exit");
      exit(-1);
    case 0:
      shm = mmap(0,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,shmid,0); //Attach Child to Shared Memory
      singleProcessMergeSort(shm,left,middle); //Use Child Labor to Sort Left Half

      munmap(shm, SIZE);//Kick Child Out
      close(shmid); //Break His Things
      exit(0);
    default:
      singleProcessMergeSort(arr,middle+1,right);//Papa Sorts The Right Side
      wait(NULL); //Make Sure Child Finished His Chores

      for(int index = 0; index <= middle; index++)//Copy Child's Work Back to Local
      {
        arr[index] = shm[index];
      }

      munmap(shm, SIZE);//Hang Up The Hat
      close(shmid);//Push the Hat Rack Over
      shm_unlink(NAME);
      merge(arr,left,middle,right);//Fusion!
  }
}
