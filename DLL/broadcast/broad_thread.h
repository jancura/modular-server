//26.05.2005 - hlavickovy subor k broad_thread.h

#ifndef	MY_BROAD_THREAD
#define MY_BROAD_THREAD


#include "stdafx.h"

#include "ou_thread.h"
#include <winsock.h>
#include <fstream>

using namespace openutils;


class recvThread : public Thread {
private :
	int port;
	std::ofstream * broad_log;
public :
	recvThread(int p, std::ofstream& log) { port = p; broad_log = &log;}
	void run();
};


#endif