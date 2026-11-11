#include "cpu.h"
#include <stdio.h>
#include <windows.h> 
typedef struct cpu_context{
    FILETIME fti;
    FILETIME ftk;
    FILETIME ftu;
    int init;
    int n;
    int i; 
}ctx;
ctx* cpu_create(void){
    ctx* ct = malloc(sizeof(*ct));
    if(!ct){
        return NULL;
    }
    ct->init = 0; 
    return ct;          
}
void cpu_destroy(ctx* ct){
    if(ct){
        free(ct);
    }
}
int cpu_init(ctx* ct, int n){
    if(!ct || n<0){
        return 0;
    }
    int code = GetSystemTimes(&ct->fti,&ct->ftk,&ct->ftu);
    if(code){
        ct->init = 1;
        ct->n = n;
        ct->i = 0; 
        return 1;
    }
    return 0; 
}
int cpu_update(void* ct1, void* out1){
    if(!(ctx*)ct1 || !((ctx*)ct1)->init){
        return 0;
    }
    ctx* ct = (ctx*) ct1;
    double* out = (double*) out1;
    FILETIME fl;
    FILETIME fk;
    FILETIME fu;
    int code = GetSystemTimes(&fl,&fk,&fu);
    if(!code){
        return 0;
    }
    ULARGE_INTEGER idu;
    idu.u.LowPart=fl.dwLowDateTime;
    idu.u.HighPart=fl.dwHighDateTime;
    ULARGE_INTEGER ctidu;
    ctidu.u.LowPart=(ct->fti).dwLowDateTime;
    ctidu.u.HighPart=(ct->fti).dwHighDateTime;

    ULARGE_INTEGER ku;
    ku.u.LowPart=fk.dwLowDateTime;
    ku.u.HighPart=fk.dwHighDateTime;
    ULARGE_INTEGER ctku;
    ctku.u.LowPart=(ct->ftk).dwLowDateTime;
    ctku.u.HighPart=(ct->ftk).dwHighDateTime;

    ULARGE_INTEGER uu;
    uu.u.LowPart=fu.dwLowDateTime;
    uu.u.HighPart=fu.dwHighDateTime;
    ULARGE_INTEGER ctuu;
    ctuu.u.LowPart=(ct->ftu).dwLowDateTime;
    ctuu.u.HighPart=(ct->ftu).dwHighDateTime;   

    ULONGLONG deltai = idu.QuadPart-ctidu.QuadPart;
    ULONGLONG deltak = ku.QuadPart-ctku.QuadPart;
    ULONGLONG deltau = uu.QuadPart-ctuu.QuadPart;

    ULONGLONG total = deltai+deltak+deltau;
    ULONGLONG busy = deltak+deltau;
    if(total==0){
        return 0; 
    }
    if(out){
        *out = (100*(double)busy/(double)total);
    }
    ct->fti = fl;
    ct->ftk= fk;
    ct->ftu = fu;
    (ct->i)++;
    // if(ct->i==ct->n){
    //     return 2;
    // }
    return 1;
}   