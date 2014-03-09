//26.05.2005 - implementacia broad_thread.cpp

#include "stdafx.h"

#include "broad_thread.h"

#include <winsock.h>
#include <fstream>

using namespace std;



#define BUFSIZE 1000

void recvThread::run()
{
	SOCKET recv_sock;
    
	if((recv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
		== INVALID_SOCKET){
			*broad_log << "neda sa vytvorit socket!" << endl;
			return;
	}
		
	sockaddr_in sockName;   
	// Zaplníme strukturu sockaddr_in
	// 1) Rodina protokolov
	sockName.sin_family = AF_INET;
	// 2) Èíslo portu, na ktorom budem vysielat broadcast
	sockName.sin_port = htons(port);
	// 3) Broadcastova adresa
	sockName.sin_addr.s_addr = INADDR_ANY;

	if(bind(recv_sock, (sockaddr*)&sockName, sizeof(sockaddr_in))
		== SOCKET_ERROR){
			*broad_log << "pomenovanie socketu bolo neuspesne!" << endl;
			closesocket(recv_sock);
			return;
	}

	char buf[BUFSIZE];
	int addrlen = sizeof(sockaddr_in);
	int size;

	if((size = recvfrom(recv_sock, buf, BUFSIZE - 1 , 0,
		(sockaddr*)&sockName, &addrlen)) == SOCKET_ERROR){
			*broad_log << "chyba pri poslani, sendto()!" << endl; 
			closesocket(recv_sock);
			return;
	}
	
	if(size > 0){
        buf[size] = '\0';
		*broad_log << "Prijaty broadcast: " << buf << endl;
		closesocket(recv_sock);
		return;
	}else{
		*broad_log << "Broadcast je prazdny!" << endl;
		closesocket(recv_sock);
		return;
	}
}
