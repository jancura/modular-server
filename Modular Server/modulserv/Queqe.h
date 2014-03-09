//pmocna trieda pre frontu sprav a jej spracovanie

#ifndef MY_QUEQE
#define MY_QUEQE

//#include "stdafx.h"

#include "Message.h"
#include "server_thread.h"
#include "broadcast_thread.h"
#include "Thread.h"
#include "Plugins.h"

#include <list>
using namespace std;

void PushBackMessageToQueqeOut(char* Sender, char* Receiver, DWORD Code, const char* Message=NULL);
void PushBackCommandToQueqeOut(char* Sender, char* Receiver, DWORD Code, const char* Message=NULL);

//void DeallocMessageQueqe();

class CMessageQueqe
{
	friend class CProcessQueqe;
public:
	CMessageQueqe() { m_bLocked=false; m_idLocker=NULL; } ;
	~CMessageQueqe() { delete GetInstance(); };

	static void PushBackMessageToQueqe(sID Sender, sID Receiver, DWORD Code, const char* Message=NULL, bool bOut=FALSE, bool bCmd=FALSE);

	static CMessageQueqe * GetInstance() 
	{
		static CMessageQueqe * Instance = new CMessageQueqe; 
		return Instance ;
	};

private:
	inline void PushMessage(sMessage Message) 
	{ 
		Lock(&(Message.nameSender)); 
		lMessages.push_back(Message); 
		UnLock(&(Message.nameSender));
	};
	bool PopMessage(sMessage& Message);

	inline void Lock(void* Locker)	 { while(m_bLocked) Sleep(TIMER); m_bLocked=true; m_idLocker=Locker; };
	inline void UnLock(void* Locker) { if(m_idLocker==Locker && m_bLocked){ m_bLocked=false; m_idLocker=NULL; } };

private:
	CMessageQueqe * m_spIam;
	void* m_idLocker;
	bool m_bLocked;
	list<sMessage> lMessages; 
};

static CMessageQueqe * pMessageQueqe = CMessageQueqe::GetInstance();


class CProcessQueqe : public Thread
{
private:
	sMessage Message;
	CClients							* m_cClients;
	recvBroadcastThread		* m_thRecvBroad;
	sendBroadcastThread		* m_thSendBroad;
	listenThread					* m_thListen;

public:
	inline void run() { Process();};
	CProcessQueqe(recvBroadcastThread* RecvBroad,sendBroadcastThread* SendBroad,listenThread* Listen,CClients* Clients);

private:
  void Process();

protected:
};

inline void DeallocMessageQueqe()
{
	if (pMessageQueqe) delete pMessageQueqe;
};


#endif