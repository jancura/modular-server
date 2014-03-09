// broadcastadd.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#include <string>

size_t broadcastadd(char * buf,size_t buf_length, size_t length)
{
	std::string c = " trala trala tralala  :-) ";
	if(buf_length >= length + c.length()){
		buf = strcat(buf, c.c_str());
		return 0;
	}
	return length + c.length();
}