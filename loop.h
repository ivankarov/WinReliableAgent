#ifndef LOOP_H
#define LOOP_H
#include <windows.h>
typedef int (*func) (void* ct, void* m);
typedef int (*func2) (void *m, HANDLE* pclient);
int loop(func f,func2 f2,void* ct, void* m,int ms,HANDLE* pclient);
#endif