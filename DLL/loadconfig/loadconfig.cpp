// loadconfig.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <fstream>
#include <string>

using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

/*void my_delete(string * co)
{
	delete co;
}
*/
//string *
size_t load(char * buf,size_t buf_length)
{
	ifstream config("config/sevice.conf");
	string line, pom;
	while(getline(config, pom)){
		line += pom;
		line.push_back('\n'); //getline neberie znak konca riadku
	}
	//return new string(line);
	if (line.length()+1<=buf_length)
	{
		strcpy(buf,line.c_str());
	}
	return line.length();
}//string loadconfig()