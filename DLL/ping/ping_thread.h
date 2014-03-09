//13.05.2005 - hlavickovy subor pre vlakna pracujuce so socketami, 
//odvodene od Tread

#ifndef MY_PING_THREAD
#define MY_PING_THREAD

#include "stdafx.h"
#include "ou_thread.h"

#include <vector>
#include <winsock.h>
#include <string>
#include <fstream>

using namespace openutils;

unsigned short checksum(unsigned char *addr, int count);

//vlakno pre posielanie ICMP paketov
class posliICMP : public Thread {
public:
	posliICMP(struct in_addr, struct in_addr, std::ofstream&);
	void run();
private:
	struct in_addr locIp;
	struct in_addr subNet;
	std::ofstream * log;
};

//vlakno pre prijem ICMP paketov
class prijmiICMP : public Thread {
public:
	prijmiICMP(std::vector<std::string>&, DWORD, std::ofstream&);
	void run();
private:
	std::vector<std::string> *zoznam;//zoznam pingnutych adries,ktore odpoveali
	DWORD timeOut; //cakacia doba pingovania
	std::ofstream * log;
};//*/

#endif