#include "ram.h"
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
typedef struct ram_context{
    DWORDLONG pcnt;
    DWORDLONG ttlkbph;
    DWORDLONG fkbph;
    int init;
    int n;
    int i; 
}rctx;
int getpcntg(DWORDLONG *pcn,DWORDLONG* ttlkbp,DWORDLONG* fkbp){
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if(!GlobalMemoryStatusEx(&statex)){
        return -1;
    }
    *pcn=statex.dwMemoryLoad;
    *ttlkbp=statex.ullTotalPhys;
    *fkbp=statex.ullAvailPhys;
    return 1;
}

rctx* ram_create(void){
    rctx* ct = malloc(sizeof(*ct));
    if(!ct){
        return NULL;
    }
    ct->init = 0; 
    return ct;          
}

int ram_init(rctx* ct, int n){
    if(!ct || n<0){
        return 0;
    }
    if(!getpcntg(&ct->pcnt,&ct->ttlkbph,&ct->fkbph)){
        return 0; 
    }
    ct->init = 1;
    ct->n = n;
    ct->i = 0; 
    return 1;
}
void ram_destroy(rctx* ct){
    if(ct){
        free(ct);
    }
}
int ram_update(void* ct1, void* out1){ // и в cpu_update() такой же syntax
    if(!(rctx*)ct1 || !((rctx*)ct1)->init){
        return 0;
    }
    rctx* ct = (rctx*) ct1;
    if(!(outr*)out1){
        return 0;
    }
    outr* out = (outr*) out1;
    DWORDLONG pc;
    DWORDLONG ttlkb;
    DWORDLONG fkb;
    if(!getpcntg(&pc,&ttlkb,&fkb)){
        return 0;
    }
    if(!pc){
        return 0; 
    }
    out->pcnt = pc;
    out->ttlkbph=ttlkb;
    out->fkbph=fkb;

    ct->pcnt = pc;
    ct->ttlkbph= ttlkb;
    ct->fkbph = fkb;
    // (ct->i)++;
    // if(ct->i==ct->n){
    //     return 2;
    // }
    return 1;
} 
