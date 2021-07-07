/************************************************************************
 * 
 * UCSC Operating Systems Assignment
 *
 * DESCRIPTION
 * An exercise in locks and deadlock prevention, various carts move around
 * a set of tracks, reserving/releasing the necessary junctions
 * 
 * This solution written by Kyle Serio.
 * Original assignment written 2020-2021 David C. Harrison.
 ************************************************************************/

#include "cartman.h"

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int junctionCheck[5] = {0};

//simple mapping of int to name
char* nameT(int num)
{
  char* name;
  switch(num)
  {
    case 0: name = "Red";
    break;
    case 1: name = "Green";
    break;
    case 2: name = "Blue";
    break;
    case 3: name = "Yellow";
    break;
    case 4: name = "Black";
  }
  return name;
}

//Simple mapping of int to name
char* nameJ(int num)
{
  char* name;
  switch(num)
  {
    case 0: name = "A";
    break;
    case 1: name = "B";
    break;
    case 2: name = "C";
    break;
    case 3: name = "D";
    break;
    case 4: name = "E";
  }
  return name;
}

//Codifies potential situations
int pairJ(int num, int color)
{
  int newNum = -1;  
  if(color == 0 && num == 0) //Red(0) has A(0) and B(1)
  {
    newNum = 1;
  }
  else if(color == 0 && num == 1) //Red(0) has A(0) and B(1)
  {
    newNum = 0;
  }
  else if(color == 1 && num == 1) //Green(1) has B(1) and C(2)
  {
    newNum = 2;
  }
  else if(color == 1 && num == 2) //Green(1) has B(1) and C(2)
  {
    newNum = 1;
  }
  else if(color == 2 && num == 2) //Blue(2) has C(2) and D(3)
  {
    newNum = 3;
  }
  else if(color == 2 && num == 3) //Blue(2) has C(2) and D(3)
  {
    newNum = 2;
  }
  else if(color == 3 && num == 3) //Yellow(3) has D(3) and E(4)
  {
    newNum = 4;
  }
  else if(color == 3 && num == 4) //Yellow(3) has D(3) and E(4)
  {
    newNum = 3;
  }
  else if(color == 4 && num == 4) //Black(4) has E(4) and A(0)
  {
    newNum = 0;
  }
  else if(color == 4 && num == 0) //Black(4) has E(4) and A(0)
  {
    newNum = 4;
  }
  return newNum;
}

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
  pthread_t cartNum;
  void* i;
}cart_data;

//Cart will arrive at a junction, and must then wait for the needed junction(s) to be freed
void arrive(unsigned int cart, enum track track, enum junction junction) 
{
  cart_data *data = malloc(sizeof(pthread_t));
  pthread_create(&data[i], NULL, parentInit, NULL);

  printf("\n\nARRIVAL: Cart[%d] - Track[%s] - Needs Junction [%s]\n",cart,nameT(track),nameJ(junction));
  int otherJ = pairJ(junction,track);
  printf("Reserving Junctions[%s] <---> [%s]\n",nameJ(junction),nameJ(otherJ));
  
  printf("Junction Reserved? [%d] <---> [%d]\n",junctionCheck[junction],junctionCheck[otherJ]);
  if(junctionCheck[junction] == 1 || junctionCheck[otherJ] == 1)
  {
    printf("Uh Oh, Waiting!\n");
    while(junctionCheck[junction] == 1 || junctionCheck[otherJ] == 1)
    {
      //Busy Wait
    }
  }

  reserve(cart,junction);
  reserve(cart,otherJ);

  junctionCheck[junction] = 1;
  junctionCheck[otherJ] = 1;

  printf("Reserved! Now crossing!\n");
  cross(cart,track,junction);
}

//Cart finishes its crossing, and can then release the locked junctions for other carts
void depart(unsigned int cart, enum track track, enum junction junction) 
{
  int otherJ = pairJ(junction,track);
  printf("FINISHED: Cart[%d] has Crossed Junctions [%s] <---> [%s]!\n",cart,nameJ(junction),nameJ(otherJ));
  printf("Releasing junctions [%s] and [%s]!\n",nameJ(junction),nameJ(otherJ));

  release(cart,junction);
  release(cart,otherJ);

  junctionCheck[junction] = 0;
  junctionCheck[otherJ] = 0;

  printf("Released!\n");
}

//empty stub
void cartman() 
{
}

