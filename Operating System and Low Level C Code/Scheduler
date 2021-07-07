/************************************************************************
 * 
 * UCSC Operating Systems Assignment
 *
 * DESCRIPTION
 * These funtions are used as callbacks to control the scheduling of
 * tasks on the cpu
 * 
 * This solution written by Kyle Serio.
 * Original assignment written 2020-2021 David C. Harrison.
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "scheduler.h"

struct node{
  char *status;
  int pause;
  int ticks;
  thread_t *thread; //tid/priority/length
  stats_t *data;//tid/waiting_time/turnaround_time/thread_count/tstats 
  struct node *next;
};

int size_of_list(int list);
void printList(int list);
void insert_ready(struct node *newNode);
void insert_io(struct node *newNode);
struct node *find(thread_t *find);
struct node *pop_front(int list);
struct node *pop_ready();
struct node *removeNode(struct node *delete);
void terminate(struct node *newNode);
void incTime();
void dispatch_certain(struct node *findT,int type);
void dispatch_new(int type);

char *algo;
int quant;
int tickCount = 0;
int readyCount = 0;

struct node *readyList = NULL;
struct node *exitList = NULL;//??
int runningID = 0;
int runTime = 0;

struct node *runningNode;
struct node *ioList;

int wait = 0;

void scheduler(enum algorithm algorithm, unsigned int quantum) 
{ 
  switch(algorithm)
  {
    case 0: algo = "FCFS";
    break;
    case 1: algo = "RR";
            quant = quantum;
    break;
    case 2: algo = "NPP";
    break;
    case 3: algo = "PP";
    break;
    case 4: algo = "NPSJF";
    break;
    case 5: algo = "PSJF";
    break;
    case 6: algo = "NPSRTF";
    break;
    case 7: algo = "PSRTF";
    break;
  }

}

//Returns the size of requested list
//0 = readyList, 1 = exitList, 2 = ioList
int size_of_list(int list)
{
  int nodes = 0;
  struct node *addList;
  if(list == 0)
  {
    addList = readyList;
  }
  else if (list == 1)
  {
    addList = exitList;
  }
  else if (list == 2)
  {
    addList = ioList;
  }

  while(addList != NULL)
  {
    nodes++;
    addList = addList->next; 
  }
  return nodes;
}

//Print out the contents of a list.
// 0 = readyList, 1 = exitList, 2 = iolist
void printList(int list)
{
  struct node *ptr;
  if(list == 0)
  {
    ptr = readyList;
    printf("readyList: ");
    while(ptr != NULL)
    {
      printf("[%d], ",ptr->thread->tid);
      ptr = ptr->next;
    }
  }
  else if(list == 1)
  {
    ptr = exitList;
    printf("exitList: ");
    while(ptr != NULL)
    {
      printf("[%d]: %d, ",ptr->thread->tid,ptr->data->tid);
      ptr = ptr->next;
    }
  }
  else
  {
    ptr = ioList;
    printf("ioList: ");
    while(ptr != NULL)
    {
      printf("[%d], ",ptr->thread->tid);
      ptr = ptr->next;
    }
  }

  printf("\n");
}

//add node to end of readyList
void insert_ready(struct node *newNode)
{
  int size = 0;
  struct node *ptr = readyList;
  if(readyList == NULL)
  {
    readyList = newNode;
  }
  else
  {
    while(ptr->next != NULL)
    {
      size++;
      ptr = ptr->next;
    }
    ptr->next = newNode;
  }
}

//add an io operation node to end of ioList
void insert_io(struct node *newNode)
{
  int size = 0;
  struct node *ptr = ioList;
  if(ioList == NULL)
  {
    ioList = newNode;
  }
  else
  {
    while(ptr->next != NULL)
    {
      size++;
      ptr = ptr->next;
    }
    ptr->next = newNode;
  }
}

//Return pointer to the requested thread's container node
struct node *find(thread_t *find)
{
  struct node *ptr = readyList;
  if(ptr == NULL)
  {
    printf("Cant find on empty list!\n");
    return NULL;
  }
  else
  {
    while(ptr != NULL)
    {
      if(ptr->thread->tid == find->tid)
      {
        return ptr;
      }
      ptr = ptr->next;
    }
  }

  return NULL;
}

//Return a pointer to the requested io thread's container node
struct node *find_io(thread_t *find)
{
  struct node *ptr = ioList;
  if(ptr == NULL)
  {
    printf("Cant find on empty list!\n");
    return NULL;
  }
  else
  {
    while(ptr != NULL)
    {
      if(ptr->thread->tid == find->tid)
      {
        return ptr;
      }
      ptr = ptr->next;
    }
  }

  return NULL;
}

//returns pointer to node w/ given thead id
struct node *find_tid(int find)
{
  struct node *ptr = ioList;
  if(ptr == NULL)
  {
    printf("Cant find on empty list!\n");
    return NULL;
  }
  else
  {
    while(ptr != NULL)
    {
      if(ptr->thread->tid == find)
      {
        return ptr;
      }
      ptr = ptr->next;
    }
  }
  return NULL;
}

//Remove and return the front of the specified list
struct node *pop_front(int list)
{
  struct node *poppedNode;
  if(list == 0 && readyList != NULL)
  {
    poppedNode = readyList;
    readyList = readyList->next;
    poppedNode->next = NULL;
  }
  else if (ioList != NULL)
  {
    poppedNode = ioList;
    ioList = ioList->next;
    poppedNode->next = NULL;
  }
  else
  {
    return NULL;
  }

  return poppedNode;
}

//Finds and returns the next ready node in the list
struct node *find_ready()
{
  struct node *poppedNode = readyList;
  if(poppedNode != NULL && strcmp(poppedNode->status,"ready") == 0)
  {
    //Grabs the first node
  }
  else if(poppedNode != NULL)
  {
    while(poppedNode != NULL && strcmp(poppedNode->status,"ready") != 0)
    {
      poppedNode = poppedNode->next;
    }
    
    if(poppedNode == NULL)
    {
      return NULL;
    }
    else
    {
      return poppedNode;
    }
  }
  
  return poppedNode;
}

//Finds and returns the highest prio item in readyList
struct node *find_ready_prio()
{
  int maxPrio = 99;
  struct node *poppedNode = readyList;
  struct node *maxPrioNode = NULL;
  if(poppedNode != NULL && strcmp(poppedNode->status,"ready") == 0 && poppedNode->next == NULL)
  {
 
  }
  else if(poppedNode != NULL)
  {
    while(poppedNode != NULL)
    {
      if(strcmp(poppedNode->status,"ready") == 0 && maxPrio > poppedNode->thread->priority)
      {
        maxPrio = poppedNode->thread->priority;
        maxPrioNode = poppedNode;
      }
      poppedNode = poppedNode->next;
    }
    
    if(maxPrioNode == NULL)
    {
      return NULL;
    }
    else
    {
      return maxPrioNode;
    }
  }
  
  return poppedNode;
}

//Finds, returns, and removes from the ready list the next thread to run
struct node *pop_ready()
{
  struct node *poppedNode = readyList;
  if(poppedNode != NULL && strcmp(poppedNode->status,"ready") == 0)
  {
    poppedNode = readyList;
    readyList = readyList->next;
    poppedNode->next = NULL;
    printf("(pop_ready - front!)popped %d\n",poppedNode->thread->tid);
  }
  else if(poppedNode != NULL)
  {
    while(poppedNode != NULL && strcmp(poppedNode->status,"ready") != 0)
    {
      poppedNode = poppedNode->next;
    }

    if(poppedNode == NULL)
    {
      printf("Couldn't find a ready thread :(\n");
      return NULL;
    }
    else
    {
      printf("Popped a non-front ready node [%d] to remove!\n",poppedNode->thread->tid);
      return removeNode(poppedNode);
    }
  }
  
  return poppedNode;
}

//Removes the given node from the readyList
struct node *removeNode(struct node *delete)
{
  struct node *poppedNode = find(delete->thread);
  struct node *prev = readyList;
  if(readyList != NULL)
  {
    if(readyList == poppedNode)
    {
      return pop_front(0);
    }
    else
    {
      while(prev->next != poppedNode)
      {
        prev = prev->next;
      }

      prev->next = prev->next->next;
      poppedNode->next = NULL;
    }
  }
  else
  {
    return NULL;
  }
  return poppedNode;
}

//Removes the given io node from the ioList
struct node *removeIO(struct node *delete)
{
  struct node *poppedNode = find_io(delete->thread);
  struct node *prev = ioList;

  if(ioList != NULL)
  {
    if(ioList == poppedNode)
    {
      return pop_front(1);
    }
    else
    {
      while(prev->next != poppedNode)
      {
        prev = prev->next;
      }

      prev->next = prev->next->next;
      poppedNode->next = NULL;
    }
  }
  else
  {
    return NULL;
  }
  return poppedNode;
}

//Kills and adds the given node to the exitList
void terminate(struct node *newNode)
{
  int size = 0;
  struct node *ptr = exitList;
  if(exitList == NULL)
  {
    exitList = newNode;
  }
  else
  {
    while(ptr->next != NULL)
    {
      size++;
      ptr = ptr->next;
    }
    ptr->next = newNode;
    ptr->data->tstats = newNode->data;
  }

  removeIO(newNode);
}

//Possible solution for time tracking >>not currently used<<
void incTime()
{
  struct node *findT = (struct node*)malloc(sizeof(struct node));
  findT = readyList;
  while(findT != NULL)
  {
    if(strcmp(findT->status,"ready") == 0 || strcmp(findT->status,"blocked") == 0)
    {
      findT->data->waiting_time++;
    }
    findT = findT->next;
  }
}

//Finds and returns the average turnaround time for each processed thread
int avgTurn()
{
  struct node *findT = (struct node*)malloc(sizeof(struct node));
  int totalTime = 0;
  int totalNodes = 0;
  findT = exitList;
  while(findT != NULL)
  {
    totalTime += findT->data->turnaround_time;
    totalNodes++;
    findT = findT->next;
  }

  return totalTime/totalNodes;
}

//Finds and returns the average time spent waiting for each thread 
int avgTime()
{
  struct node *findT = (struct node*)malloc(sizeof(struct node));
  int totalTime = 0;
  int totalNodes = 0;
  findT = exitList;
  while(findT != NULL)
  {
    totalTime += findT->data->waiting_time;
    totalNodes++;
    findT = findT->next;
  }

  return totalTime/totalNodes;
}

//Dispatches the specified thread to the processor
void dispatch_certain(struct node *findT, int type)
{
  if(type == 0 && runningID == 0)//  SYS_EXEC
  {
    findT->status = "running";
    runningID = findT->thread->tid;
    runningNode = findT;
    insert_io(removeNode(findT));
    sim_dispatch(findT->thread);
  }
  else if(type == 1)//  READ/WRITE
  {
    if(strcmp(algo,"NPP") == 0)
    {
      wait = 1;
    }
    
    runningID = 0;
    findT->status = "blocked";
    findT->pause = sim_time() + 1;
  }
  else if(type == 2)//  IO_COMPLETE
  {
    findT->status = "ready";
    findT->pause = sim_time() + 1;
    insert_ready(removeIO(findT));
  }
}

//Finds and dispatches the next ready to run thread
//Behavior changes based on chosen algorithm
void dispatch_new(int type)
{
  struct node *findT;
  int preempt = 0;

  if(type == 2)
  {
    findT = find_ready_prio();
  }
  else
  {
    findT = find_ready();
  }

  if(strcmp(algo,"PP") == 0 && runningID != 0 && findT != NULL)
  {
    struct node *temp = find_tid(runningID);
    if(temp->thread->priority > findT->thread->priority)
    {
      preempt = 1;
    }
  }

  if(findT != NULL && (type == 0 || type == 2) && runningID == 0)//  TICK
  { 
    findT->status = "running";
    runningID = findT->thread->tid;
    findT->data->waiting_time += sim_time() - findT->pause;
    runningNode = findT;
    insert_io(removeNode(findT));
    findT->ticks = quant;
    sim_dispatch(findT->thread);
  }  
  else if((type == 1 || type == 2) && (strcmp(algo,"RR") == 0 || preempt == 1))// RR Handling, every tick calls this scheduler
  {
  
    if(runningID != 0)//something running
    {
      struct node *temp = find_tid(runningID);
      
      if(temp->ticks == 0 || preempt == 1)//run out of quantum ticks
      {  
        temp = removeIO(find_tid(runningID));
        temp->status = "ready";
        temp->pause = sim_time();
        temp->ticks = quant;
        insert_ready(temp);//puts runningID back on ready
        runningID = 0;
       if(preempt == 1 && strcmp(algo,"PP") == 0)
        {
          preempt = 0; 
          dispatch_certain(findT,0);
        }
        else
        {
          dispatch_new(0);
        }
      }
    }
    else
    {
      dispatch_new(0);
    }
  }
}

//Possible solution for dispatching alterations by algo type <<not in use>>
void sim_ready()
{
  readyCount++;
  if(strcmp(algo,"RR") == 0)
  {
    dispatch_new(1);
  }
  else if(strcmp(algo,"NPP") == 0 || strcmp(algo,"PP") == 0)
  {
    dispatch_new(2);
  }
  else
  {
    dispatch_new(0);
  }
}

//Possible solution for tick based dispatch >>Not currently used<<
void sim_tick() 
{

  if(runningID != 0 && strcmp(algo,"RR") == 0)
  {
    struct node *temp = find_tid(runningID);
    temp->ticks--;
  }

  if(strcmp(algo,"NPP") == 0)
  {
    if(wait == 1)
    {
      wait = 0;
    }
    else
    {
      dispatch_new(2);
    }
    
  }
  else if(strcmp(algo,"PP") == 0)
  {
    dispatch_new(2);
  }
  else
  {
    dispatch_new(0);
  }
  
  tickCount++;
}

//Both creates a node for and executes the given thread by placing it on the cpu
void sys_exec(thread_t *t) 
{ 
  //printf("SYS_EXEC CALLED\n");
  struct node *newThread = (struct node*)malloc(sizeof(struct node));
  stats_t *newData = (stats_t*)malloc(sizeof(stats_t));
  newThread->thread = t;
  newThread->status = "ready";

  newData->tid = newThread->thread->tid;
  newData->waiting_time = 0;
  newData->turnaround_time = sim_time();
  newThread->data = newData;
  newThread->pause = sim_time();
  newThread->ticks = quant;

  insert_ready(newThread);

  if(strcmp(algo,"NPP") == 0)
  {
    
  }
  else
  {
    dispatch_certain(newThread,0);
  }
  
}

//Dispatch a read op
void sys_read(thread_t *t) 
{ 
  struct node *findT;
  findT = find_io(t);
  dispatch_certain(findT,1);
}

//Dispatch a write op
void sys_write(thread_t *t) 
{
  struct node *findT;
  findT = find_io(t);
  dispatch_certain(findT,1);
}

//Terminates and adds to exitList the given thread
void sys_exit(thread_t *t)
{ 
  struct node *findT;
  findT = find_io(t);

  findT->status = "terminated";
  findT->data->turnaround_time = sim_time() + 1 - findT->data->turnaround_time;
  terminate(findT);
  runningID = 0;
  if(strcmp(algo,"NPP") == 0)
  {
    wait = 1;
  }
}

//Marks and completes a given io op
void io_complete(thread_t *t) 
{
  struct node *findT;
  findT = find_io(t);
  dispatch_certain(findT,2);
}

//Marks and begins writing an io thread
void io_starting(thread_t *t) 
{ 
  struct node *findT;
  findT = find_io(t);
  findT->status = "writing";
  findT->data->waiting_time += sim_time() - findT->pause;
}

//Uses the exitList to find the stats across all executed tasks for a summary
stats_t *stats() 
{ 
  int thread_count = size_of_list(0) + size_of_list(1) + size_of_list(2);
  stats_t *stats = malloc(sizeof(stats_t));
  stats->tstats = malloc(sizeof(stats_t)*thread_count);

  stats_t *ptr = exitList->data;
  int index = 0;

  //ptr goes through our exitList to assign to the new tstats
  while(ptr != NULL)
  {
    stats->tstats[index].tid = ptr->tid;
    stats->tstats[index].waiting_time = ptr->waiting_time;
    stats->tstats[index].turnaround_time = ptr->turnaround_time;
    ptr = ptr->tstats;
    index++;
  }

  stats->waiting_time = avgTime();
  stats->turnaround_time = avgTurn();
  stats->thread_count = thread_count;

  return stats; 
}
