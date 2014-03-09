	//26.05.2005 - implementacia broadcast_thread.cpp

#include "stdafx.h"

#include "broadcast_thread.h"

#include <winsock.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

CBroadLog BroadLog(pcProgramFolder);

/******************************************************************************
 * konstruktor sendBroadcastThread::sendBroadcastThread()
 ******************************************************************************/
sendBroadcastThread::sendBroadcastThread(struct in_addr b,int p,const string& m,DWORD time)
{
	broadIP = b;
	port = p;
	sprava = m;
	terminate = false;
	timeOut = time;
}// sendBroadcastThread::sendBroadcastThread()


/******************************************************************************
 * void sendBroadcastThread::run()
 ******************************************************************************/
void sendBroadcastThread::run()
{
	SOCKET broad_sock;	
	if((broad_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
		== INVALID_SOCKET){
			BroadLog.Write( this, "Send: Neda sa vytvorit socket pre broadcast!");
			return;
	}

	struct sockaddr_in sockName;   
	// Zaplníme strukturu sockaddr_in
	// 1) Rodina protokolov
	sockName.sin_family = AF_INET;
	// 2) Císlo portu, na ktorom budem vysielat broadcast
	sockName.sin_port = port;
	// 3) Broadcastova adresa
	sockName.sin_addr = broadIP;

	//zacneme vysielat broadcast spravu
	while(!terminate){
		if(sendto(broad_sock, sprava.c_str(), sprava.size(), 0,
			(sockaddr*)&sockName, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
				BroadLog.Write( this, "Send: Chyba pri poslani broadcastu, sendto()!"); 
				closesocket(broad_sock);
				return;
		}
		Sleep(timeOut);
	}
	closesocket(broad_sock);
}// void sendBroadcastThread::run()


/******************************************************************************
 * void recvBroadcastThread::run()
 ******************************************************************************/
void recvBroadcastThread::run()
{
	
	if((recv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
		== INVALID_SOCKET){
			BroadLog.Write( this, "Recv: Neda sa vytvorit socket!");
			return;
	}

	sockaddr_in sockName;   
	// Zaplníme strukturu sockaddr_in
  // 1) Rodina protokolov
  sockName.sin_family = AF_INET;
  // 2) Císlo portu, na ktorom budem vysielat broadcast
  sockName.sin_port = port;
  // 3) Broadcastova adresa
  sockName.sin_addr.s_addr = INADDR_ANY;

	if(bind(recv_sock, (sockaddr*)&sockName, sizeof(sockaddr_in)) 
		== SOCKET_ERROR)
	{
		BroadLog.Write( this, "Recv: Pomenovanie socketu bolo neuspesne!");
		return;
	}

	char buf[BUFSIZE];
	int addrlen = sizeof(sockaddr_in);
	int size;

	while(true){ //ukoncuje sa zavretim socketu recvfrom
        if((size = recvfrom(recv_sock, buf, BUFSIZE - 1 , 0,(sockaddr*)&sockName, 
			&addrlen)) == SOCKET_ERROR){
				//ak bol socket zavrety v sendBroad tak vracia chybu WSAEINTR
				if(WSAGetLastError() != WSAEINTR){
					CLog::WriteLog(this, "RecvBroadcast: Chyba pri prijati broadcastu, recvfrom()!");
					return;
				}else return;
		}
		buf[size] = '\0';
		//if(sockName.sin_addr.s_addr != localhost.s_addr){
		char Log[BUFSIZE];
		sprintf(Log, "Recv: Prijaty broadcast od: %s obsah spravy: %s",inet_ntoa(sockName.sin_addr),buf);
    BroadLog.Write( this, Log); 
		//}
	}
}// void recvBroadcastThread::run()

void CBroadLog::Write(void* Who, char * pLog)
{
	Lock(Who);
	broad_log.open(pLogFile, ios::in);
	broad_log << pLog << endl;
	broad_log.close();
	Unlock(Who);
}