// broadcast.cpp : Defines the entry point for the DLL application.
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
#include <fstream>
#include <string>
#include "broad_thread.h"

using namespace std;
//using namespace openutils;

//nastavenie portu pre prijem a odoslanie
int port = 9009;

/******************************************************************************
 * bool broadcast(struct in_addr broadIP)
 ******************************************************************************/
bool broadcast(struct in_addr broadIP, char * log)
{
	ofstream broad_log(log);
	string sprava = "ta ci pana, ide to!  :-) ";
	//inicializacia socketov
	WSADATA data;
	WORD wVersionRequired = MAKEWORD(2,2);
	if(WSAStartup(wVersionRequired, &data) != 0){
		broad_log << "Nepodarilo sa inicializovat sockety pre novy broadcast!"<<endl;
		return false;
	}

	
    SOCKET broad_sock;

	if((broad_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
		== INVALID_SOCKET){
			broad_log << "neda sa vytvorit socket pre novy broadcast!" << endl;
			WSACleanup();
			return false;
	}
	

	struct sockaddr_in sockName;   
	// Zaplníme strukturu sockaddr_in
	// 1) Rodina protokolov
	sockName.sin_family = AF_INET;
	// 2) Èíslo portu, na ktorom budem vysielat broadcast
	sockName.sin_port = htons(port);
	// 3) Broadcastova adresa
	sockName.sin_addr = broadIP;

	recvThread * recieve = new recvThread(port, broad_log);
	try{
		recieve->start();
		Sleep(20);
		//posleme broadcast spravu
		if(sendto(broad_sock, sprava.c_str(), sprava.size(), 0,
			(sockaddr*)&sockName, sizeof(sockaddr_in)) == SOCKET_ERROR){
				broad_log << "chyba pri poslani, sendto()!" << endl; 
				closesocket(broad_sock);
				WSACleanup();
				return false;
		}
		recieve->stop();
	}catch (ThreadException ex) {
		broad_log << ex.getMessage() << endl;
		closesocket(broad_sock);
		delete recieve;
		WSACleanup();
		return false;
	}

	closesocket(broad_sock);
	delete recieve;
	WSACleanup();
	return true;
}//bool broadcast(struct in_addr broadIP)

