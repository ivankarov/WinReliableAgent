#ifndef WINSERVICE_H
#define WINSERVICE_H
#include <windows.h>
int InitSvc(SC_HANDLE *srv);
BOOL DelSvc(SC_HANDLE srv);
VOID ReportSvc(DWORD dwCurrentState, DWORD dwWin32ExitCode,DWORD dwWaitHint);
#endif