// Chat.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#include "ou_thread.h"
#include <WinSock2.h>
#include "ChatThread.h"

using namespace openutils;

CClientList ClientList;

void Function()
{
	//inicializacia socketov
	WSADATA data;
	WORD wVersionRequired = MAKEWORD(2,2);
	if(WSAStartup(wVersionRequired, &data) != 0){
		return; //Nepodarilo sa inicializovat socket
	}
	ClientList.start();
	Listen();
	ClientList.Stop();

	WSACleanup();
}
