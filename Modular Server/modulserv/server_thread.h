//14.05.2005 - hlavickovy subor pre server_thread.cpp
// deklaruje vlakna odvedene z ou_thread pre TCP_SERVER na pocuvanie a 
// komunikovanie

/*Pali Jancura - to jest ja :)
 * 19.05.2005
 *	chng:	class comunicateThread : public Thread 
 *				- priadne polozka public: char message[BUFSIZE]; sluzi na 
 *				  na ulozenie spravy prichadzajucej od klienta, s ktorym dane
 *				  vlakno komunikuje,public aby som mohol k nej priamo pristupovat
 * 17.05.2005
 *	add:	void prepis_vector() - deklaracia
 *	rem:	bool sleduj_close() - deklaracia
 *	add:	bool sleduj_close() - deklaracia
 *	rem:	class startComunicateThread : public comunicateThread 
 *	chng:	class comunicateThread : public Thread 
 *				- pridana polozka public: int id, pre id vlakna
 *				- polozka socket presunuta z private do public
 *				- pridana polozka listenThread* father; ukazatel na otca vlakna
 * 15.05.2005
 *	add:	class startComunicateThread : public comunicateThread 
 *			class stopComunicateThread : public comunicateThread 
 * 14.05.2005
 *	add:	class listenThread : public Thread 
 *			class comunicateThread : public Thread 
 */

#ifndef MY_SERVER_THREAD
#define MY_SERVER_THREAD

#include "stdafx.h"

#include <winsock.h>
#include <vector>
#include <string>

#include "telnet.h"
#include "broadcast_thread.h"
#include "ou_thread.h"
#include "Message.h"
//#include "Thread.h"

using namespace openutils;

//int  GetClientsID(int * clients);

//#define BUFSIZE 1000

/******************************************************************************
 * vlakno pre zastavenie valkien communicateThread ulozenych vo vectore vlakien
 *****************************************************************************/
/*class stopCommunicateThread : public Thread {
private:
	sendBroadcastThread * sendBroad;
	char cLog[BUFSIZE];		//buffer pre log

public:		//verejne funkcie
	stopCommunicateThread::stopCommunicateThread() 
	{
		sendBroad = NULL;
		terminate = false;
	}
	stopCommunicateThread::stopCommunicateThread(sendBroadcastThread * send)
	{
		sendBroad = send;
		terminate = false;
	}
	void run();
	inline void Process(sMessage Message){;};

public:		//premenne pre volny pristup
	bool terminate;	//info, ze vlakno sa ma ukoncit (ukonci sa cyklus while())

}; //class stopCommunicateThread

//*/

//class CShowClients;

/******************************************************************************
 * vlakno pre komunikaciu s klientom, ktory sa napojil na server
 *****************************************************************************/
class communicateThread : public Thread {
	friend class CClients;
private:
	bool Initialize();
	bool Negotiation();
	void AnalyzeError();
	bool AcceptMessage(std::string time, int&, char []);

	CProcCmdThread* FindCmdProcess(char * pName);
	void StopCommandsThread();
	inline void StopCommandProcessMessageThread()
	{
		while (vecCommandMessageThread.size()>0) {
			if (vecCommandMessageThread[0]->IsRunning()) {
				vecCommandMessageThread[0]->stop();
			}
			CLog::WriteLog(this, "StopCommandThraed(): delete vlakno");
			delete vecCommandMessageThread[0];
			CLog::WriteLog(this, "StopCommandThraed(): erase vlakno");
			vecCommandMessageThread.erase(vecCommandMessageThread.begin());
		}
	};
private:		
//	CProcessThread * thProcess;
	struct sockaddr_in clientInfo;  //info o klientovi, pre ktory bolo vlakno spustene
	struct in_addr localhost;				// IP localhostu, na  ktorom server bezi

  bool m_bTelnet;
	bool m_bPutty;
	Telnet telnet;

	SOCKET socket;	//socket vlakna

	CODE end;				//info, ze vlakno dobehlo a ma byt ukoncene
	CODE terminate;	//info, ze vlakno sa ma ukoncit (ukonci sa cyklus while())
	sID  m_sID;			//id vlakna

	std::vector<CCommandThread*>  vecCommandThread;
	std::vector<CProcCmdThread*>  vecCommandMessageThread;

public:		//verejne funkcie
	communicateThread(SOCKET, struct sockaddr_in, struct in_addr, int/*, CShowClients**/);

	char* getIP();
	void  run();
	void  Process(sMessage Message);
	void  Execute(std::vector<std::string> command);
	void  Send(const char* pData, int iLen);
	void  ProcessCommandMessage(sMessage Message);
	void	RunOutCommand(const char * Command, CommandHandler hndCommand);

public:		//premenne 
	char message[BUFSIZE];	//buffer pre prijatie spravy od klienta, pre ktore vlakno bezi
	int recvsize;	//velkost prijata do message
};//class communicateThread 


/******************************************************************************
 * deklaracia funkcii, pouzitych mimo vlakien, musia byt v server_thread.cpp,
 * pretoze pracuju s vectorom vlakien commmunicateThread, zatvaraju im sockety
 ******************************************************************************/
//void zavri_socket();
//void zavri_socket(int id);

/******************************************************************************
* trieda pre pocuvanie ukladanie pripojenych klientov a pracu s nimi
*****************************************************************************/
class CClients
{
public:
	CClients() : /*thProcess(NULL),*/ m_idLocker(NULL), m_bLocked(false) {m_sId.type=T_PCHAR; strcpy(m_sId.t_name, "Clients");};
//	~CClients(){if(thProcess) delete thProcess;};
	inline void Lock(void* Locker)	{ while(m_bLocked) Sleep(TIMER); m_bLocked=true; m_idLocker=Locker; };
	inline void UnLock(void* Locker) { if(m_idLocker==Locker && m_bLocked){ m_bLocked=false; m_idLocker=NULL; } };
private:	
	void *	m_idLocker;
	bool	m_bLocked;
	sID		m_sId;
	std::vector<communicateThread*> client_list;
	//CProcessThread * thProcess;
public:
	void	Process(sMessage Message);
	bool	Process(void* Who, index i, sMessage Message);

	bool	PushClient(void* Who, communicateThread* client);
	size_t	GetSize(void* Who);
	int		GetClientId(void* Who, index i);

	//extern int GetClientsID(int* clients);

private:
	void	ClientEnd(void* Who, index i, bool bCloseSocket=false);
	index	GetIndex(void* Who, int id);
	std::string FillInfoWho(sID ID);
	SOCKET	GetClientSocket(void* Who, index i);
	char *	GetClientIP(void* Who, index i);
	CODE	GetClientEnd(void* Who, index i);
	CODE	GetClientTerminate(void* Who, index i);
	bool	SetClientEnd(void* Who, index i, bool b);
	bool	SetClientTerminate(void* Who, index i, bool b);
	bool	IsClientTelnet(void* Who, index i);
};

extern CClients Clients;
 //int CClients::GetClientsID(int* clients);

/*
class CShowClients
{
public:
	CShowClients():m_Clients(NULL){};
	CShowClients(CClients* cClients) {m_Clients=cClients;};

	int GetClientsID(int * clients);

private:
	CClients * m_Clients;
};

/******************************************************************************
* vlakno pre pocuvanie na zadanom porte
*****************************************************************************/
class listenThread : public Thread 
{
private:
private:	//nespristupnene premenne
	int port; //port na ktorom pocuva listen() vo vlakne listenThead
	struct in_addr localhost; //IP localhostu
	//char cLog[BUFSIZE];		//buffer pre log
	CClients* m_cClients;


public:	//verejne funkcie
	listenThread(int, struct in_addr, CClients*);
	~listenThread(){};
	void run();
	void Process(sMessage Message);
	
public:  //premenne pre volny pristup
	//stopCommunicateThread* stopComm; //ukazatel na vlakno, ktore ukoncuje beh 
	//vlakien communicateThread, koli tomu ak
	//dojde k ukonceniu servru vo vnutri 
	//vlakna listenThread alebo jeho syna
	SOCKET serv_sock; //socket, na ktorom bezi listen(), a teda prijem servru
	bool terminate;	//info, ze vlakno sa ma ukoncit (ukonci sa cyklus while())

};//class listenThread 

#endif