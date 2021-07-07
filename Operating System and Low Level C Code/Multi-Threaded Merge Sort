/************************************************************************
 * Multi-Threaded Merge Sort
 *
 * UCSC Operating Systems Assignment 1
 * 
 * This solution written by Kyle Serio.
 * Original assignment written 2020-2021 David C. Harrison.
 ************************************************************************/


#include "merge.h"

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/* LEFT index and RIGHT index of the sub-array of ARR[] to be sorted */
void singleThreadedMergeSort(int arr[], int left, int right) 
{
  if (left < right) {
    int middle = (left+right)/2;
    singleThreadedMergeSort(arr, left, middle); 
    singleThreadedMergeSort(arr, middle+1, right); 
    merge(arr, left, middle, right); 
  } 
}

typedef struct
{
  pthread_t thread_id;
  pthread_t tid;
  void* i;
  int *array;
  int left;
  int right;
}thread_data;

/*Helper Function for Creating Threads*/
void *threadInit(void *dataPtr)
{
  thread_data *data = (thread_data*)dataPtr;
  singleThreadedMergeSort(data->array,data->left,data->right);
  pthread_exit(NULL);
}

void *parentInit(void *dataPtr)
{
  thread_data *data = (thread_data*)dataPtr;
  void* res;
  int RM = (data->left+data->right)/2;
  thread_data childData;
  childData.tid = data->tid+1;

  childData.left = RM + 1;
  childData.right = data->right;
  childData.array = data->array;
  pthread_create(&childData.tid, NULL, threadInit, (void *)&childData);//CREATE and SORT FARRIGHT section

  singleThreadedMergeSort(data->array,data->left,RM); //MERGE midright section PARENT

  pthread_join(childData.tid, &res);//WAIT for child thread
  merge(data->array,data->left,RM,data->right);

  pthread_exit(NULL);
}

void multiThreadedMergeSort(int arr[], int left, int right) 
{
  void* res;
  thread_data data[2];
  int middle = (left+right+1)/2;
  int LM = (left+middle)/2;

  data[0].left = LM+1;
  data[0].right = middle;
  data[1].left = middle+1;
  data[1].right = right;

  for(long i = 0; i < 2; i++)
  {
    data[i].array = arr;
    data[i].tid = i;
    if(i == 1)
    {
      int rc = pthread_create(&data[i].tid, NULL, parentInit, (void *)&data[i]);
      if (rc)
      {
        printf("Error! I think...\n");
      }
    }
    else
    {
      int rc = pthread_create(&data[i].tid, NULL, threadInit, (void *)&data[i]);
      if (rc)
      {
        printf("Error! I think...\n");
      }
    }
  }

  singleThreadedMergeSort(arr,left,LM);
  pthread_join(data[0].tid, &res); //WAIT for midleft section's thread
  merge(arr,left,LM,middle);//Merge parent's leftmost section with leftmid section
  pthread_join(data[1].tid, &res);//WAIT for rightmid section's thread
  merge(arr,left,middle,right); //merge left and right halves
}
