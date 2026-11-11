#ifndef RAM_H
#define RAM_H
#include <windows.h>
typedef struct ram_context rctx;
typedef struct ram_contextout{
    DWORDLONG pcnt;
    DWORDLONG ttlkbph;
    DWORDLONG fkbph;
}outr;
rctx* ram_create(void);
int ram_init(rctx* ct, int n);
void ram_destroy(rctx* ct);
int ram_update(void* ct1, void* out1);
#endif