// echo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#include "Handler.h"
#include <string>
#include <winsock.h>

#define ECHO 1
#define CR 13
#define LF 10

typedef void (* tPushMsgHandler )(char* Sender, char* Receiver, DWORD Code, const char* Message); 

size_t GetName(char* pName, size_t iSize)
{
	size_t size=0; std::string echo="$echo";
	if(iSize>echo.size()){
		strcpy(pName, echo.c_str());
		return 0;
	}
	return echo.size();
}

void Command(int clients[], int iSize, SOCKET socket, int Id, tPushMsgHandler fPushMessage, char * pParameters)
{
	std::string message; //WORD newline = CR << 8; newline |= LF; 
	if (pParameters) {
		message=pParameters;
		message.push_back(CR);message.push_back(LF);
		message.append("--> ");
	} else {
		message="";
		message.push_back(CR);message.push_back(LF);
		message.append("--> ");
	}
	
	fPushMessage("echo", "Clients", ECHO, message.c_str());	

}
