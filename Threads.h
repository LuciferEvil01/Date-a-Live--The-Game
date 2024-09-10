#ifndef Threads
#define Threads

#include"Main.h"
#pragma once

extern pthread_t threads[100];
extern bool usingThread[100];
extern int threadPointer;
extern bool restart;


void killAll();
void Add_Thread(void* Function);

#endif