// ping.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//26.05.2005 - implementacia ping.cpp, presunute sem do dll

#include "ping.h"
#include "ping_thread.h"

#include <winsock.h>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool ping(struct in_addr localIP, struct in_addr subnet, DWORD timeOut, 
																char * ping_log)
{
	ofstream log(ping_log);
	vector<string> adresar;
	Thread *t1 = new prijmiICMP(adresar, timeOut, log);
	Thread *t2 = new posliICMP(localIP, subnet, log);

    try {
        t1->start();
        t2->start();
        t2->stop();
		t1->stop();
	}catch(ThreadException ex) {
            log << ex.getMessage() << endl;
			return false;
	} 
	delete t1;
	delete t2;

	for(vector<string>::size_type i = 0; i < adresar.size(); ++i){
        log << adresar[i] << endl;
	}

	return true;
}