// ProcessEcho.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#include <winsock.h>

#define ECHO 1

int GetCode()
{
	return ECHO;
}

void ProcessCommandMessage(const char* Sender, const char* Receiver, int clients[], int iSize, SOCKET socket, int Code, const char* Data)
{
	send(socket, Data, strlen(Data)+1, 0);
}