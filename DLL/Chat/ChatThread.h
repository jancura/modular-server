//

#ifndef _CHAT_THERAD_H_
#define _CHAT_THERAD_H_

#include "stdafx.h"
#include <vector>

#include "winsock2.h"
#include "ou_thread.h"
using namespace openutils;

#define TIMER 20
#define BUFSIZE 1000

void Listen();

class CClientThread : public Thread
{
public:
	CClientThread(SOCKET Socket, int ID, sockaddr_in ClientInfo) 
		: m_iID(ID), m_Socket(Socket), m_bTerminate(false) {m_ClientInfo=ClientInfo;};
	void run();

	void inline Stop() { closesocket(m_Socket); stop();};
	inline bool IsTerminate() {return m_bTerminate;};
	inline SOCKET GetSocket() {return m_Socket;};

private:
	int m_iID;
	SOCKET m_Socket;
	sockaddr_in m_ClientInfo;
	bool m_bTerminate;
};


class CClientList : public Thread
{
public:
	CClientList():m_bLocked(false), m_pLocker(NULL), m_bTerminate(false) {};
	~CClientList() 
	{
		while (m_vecClientList.size()>0) {
			m_vecClientList[0]->Stop();
			if(m_vecClientList[0]) delete m_vecClientList[0];
			m_vecClientList.erase(m_vecClientList.begin());
		}
	};

	inline void run()
	{
		while (!m_bTerminate)
		{
			Lock(this);
			Pop();
			UnLock(this);
			Sleep(TIMER);
		}
		//StopVecClient();
	};

	void Lock(void* Locker) { while(m_bLocked) Sleep(TIMER); m_bLocked=true; m_pLocker=Locker; };
	void UnLock(void* Locker ){ if(m_pLocker==Locker && m_bLocked){ m_bLocked=false; m_pLocker=NULL; } };
	inline void Send(void* Who, char* pMessage, int iID)
	{
		if(m_bLocked && m_pLocker==Who){
			for (size_t i=0; i<m_vecClientList.size(); ++i) {
				char Message[BUFSIZE+10];
				int Size = sprintf(Message, "%i: %s%c", iID, pMessage,'\0');
				send(m_vecClientList[i]->GetSocket(), Message, Size, 0);
			}
		}
	};
	inline void Push(void * Who, CClientThread* Client) 
	{
		if(m_bLocked && m_pLocker==Who){
			m_vecClientList.push_back(Client);
		}
	};

	inline void Stop()
	{
		m_bTerminate=true;
		StopVecClient();
		stop();
	};

private:
	inline void Pop() 
	{	 
		for (size_t i=0; i<m_vecClientList.size(); ++i) {
			if (m_vecClientList[i]->IsTerminate()) {
				m_vecClientList[i]->stop();
				delete m_vecClientList[i];
				m_vecClientList.erase(m_vecClientList.begin()+i);
				--i; //zmena indexovania
			}
		}
	};

	inline void StopVecClient()
	{
		Lock(this);
		while (m_vecClientList.size()>0) {
			m_vecClientList[0]->Stop();
			if(m_vecClientList[0]) delete m_vecClientList[0];
			m_vecClientList.erase(m_vecClientList.begin());
		}
		UnLock(this);
	};

private:
	bool m_bLocked;
	void* m_pLocker;
	std::vector<CClientThread*> m_vecClientList;
	bool m_bTerminate;
};

extern CClientList ClientList;

#endif