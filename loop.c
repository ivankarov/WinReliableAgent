#include "loop.h"
#include <windows.h>
#include <stdio.h>

int loop(func f,func2 f2,void* ct, void* m,int ms,HANDLE* pclient){
    while(1){
        Sleep(ms);
        int code = f(ct,m);
        if(!code){
            f2(m,pclient);
            return 0;
        }
        if(code == 1){
            f2(m,pclient);
        }
        if(code == 2){
            f2(m,pclient);
            return 1; 
        }
    }
    return 0;
}