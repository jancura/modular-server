//26.05.2005 - hlavickovy subor pre definovanie broadcastobych vlakien pre
// a vysielanie broadcastu

#ifndef	MY_BROADCAST_THREAD
#define MY_BROADCAST_THREAD

#include "stdafx.h"

#include <string>
#include <winsock.h>

#include "Thread.h"
#include "ou_thread.h"
#include "Message.h"

using namespace openutils;


class recvBroadcastThread : public Thread {
private:
	int port;
	struct in_addr localhost;
public:
	recvBroadcastThread(int p, struct in_addr localIP) 
	{
		port = p; 
		localhost = localIP;
	}
	void run();
	inline void Process(sMessage Message)
	{
		if (Message.Out && !Message.Command) {
			RunOutProcess(Message);
		} else {
			if (!Message.Command) {
				switch(Message.MessageCode) {
				case END:
					closesocket(recv_sock);
					break;
				default:
					return;
				}
			}
		}
	};
public:
	SOCKET recv_sock;
};


class sendBroadcastThread : public Thread {
private:
	struct in_addr broadIP;
	int port;
	std::string sprava;
	DWORD timeOut;
	bool terminate;
	char pBroadLogFile[MAX_PATH];
public:
	sendBroadcastThread(struct in_addr, int, const std::string&, DWORD);
	void run();
	inline void Process(sMessage Message)
	{
		if (Message.Out && !Message.Command) {
			RunOutProcess(Message);
		} else {
			if (!Message.Command) {
				switch(Message.MessageCode) {
				case END:
					terminate=true;
					break;
				default:
					return;
				}
			}
		}
	};
public:
};

class CBroadLog
{
public:
	CBroadLog(char * pFolder) { sprintf(pLogFile, "%sLog\\broadcast.log", pFolder); }; 
	void Write(void* Who, char * pLog);
private:
	inline void Lock(void* pLocker) { while(m_bLocked) Sleep(TIMER); m_bLocked=TRUE; m_pLocker=pLocker; };
	inline void Unlock(void* pLocker) { if(pLocker==m_pLocker) m_bLocked=FALSE; };
private:
	BOOL m_bLocked;
	void * m_pLocker;
	std::ofstream broad_log;
	char pLogFile[MAX_PATH];
};

#endif