#pragma once
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")
#include <windows.h>
#include <winsvc.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
SERVICE_STATUS gSvcStatus;
SERVICE_STATUS_HANDLE gSvcStatusHandle;
VOID WINAPI ServiceCtrlHandler(DWORD ctrlCode) {
    switch (ctrlCode) {
        case SERVICE_CONTROL_STOP:
            if (gSvcStatus.dwCurrentState != SERVICE_RUNNING) break;
            gSvcStatus.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
            break;
        default: break;
    }
}
int InitSvc(SC_HANDLE* srv){
    FILE *fp = fopen("D:\\agent\\file1.txt","w");
    if(!fp){
        return 0;
    }
    gSvcStatusHandle = RegisterServiceCtrlHandler(TEXT("AGENTICHE"), ServiceCtrlHandler);
    SC_HANDLE scmngr = OpenSCManagerA(NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
    if(!scmngr){
        fprintf(fp,"manager failed: %lu\n",GetLastError());
        return -2;
    }
    SC_HANDLE scsrv =NULL;
    scsrv = OpenServiceA(scmngr,"AGENTICHE",SC_MANAGER_ALL_ACCESS);
    if(scsrv){
        fprintf(fp,"service is already exist or - %d \n ",GetLastError());
        *srv = scsrv;
        CloseServiceHandle(scmngr);
        return 1; 
    }
    else{
        fprintf(fp,"service isnt exist");
    }
    scsrv = CreateServiceA(scmngr,"AGENTICHE","AGENTICHE",SC_MANAGER_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,"d:\\agent\\agent.exe",NULL,NULL,NULL,NULL,NULL);
    if(!scsrv){
        CloseServiceHandle(scmngr);
        fprintf(fp,"service failed: %lu\n",GetLastError());
        return -1;
    }
    *srv = scsrv;
    fprintf(fp,"All is ok \n");
    CloseServiceHandle(scmngr);
    fclose(fp); 
    return 1;  
}
BOOL DelSvc(SC_HANDLE srv){
    BOOL result = DeleteService(srv);
    CloseServiceHandle(srv);
    return result;
}
VOID ReportSvc(DWORD dwCurrentState, DWORD dwWin32ExitCode,DWORD dwWaitHint){
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;
    gSvcStatus.dwCurrentState = dwCurrentState;
    SetServiceStatus(gSvcStatusHandle,&gSvcStatus);
}


