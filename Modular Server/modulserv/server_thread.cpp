//14.05.2005 - implementacia run() funkcii pre vlakna vytvorene serverom

/* Pali Jancura - to jest ja :)
 * 14.11.2005
 *	- dalsie obmeny pri nicializacii spojenia s klientom pomocou TELNETU
 * 12.11.2005
 *	- prve pokusy o TELNET
 * 23.05.2005
 *	chng:	void zavri_socket(id) - uz by malo fungovat spravne
 * 19.05.2005
 *	add:	char* communicateThread::getIP() - vrati IP, daneho vlakna
 *	chng:	void communicateThread::run() - drobne zmeny,pridany v nom execute()
 * 17.05.2005
 *	chng:	void communicateThread::run() 
 *			void listenThread::run()
 *	add:	void zavri_socket() - ukoncuje sa spojenie zavretim socketov
 *			void zavri_socket(id) 
 *				- ukoncuje sa spojenie jedneho socketu podla id
 *				- nie je dobry,index sa totiz meni po vypusteni nejakeho vlakna,
 *				  client_list[id-1] teda obecne nevrati vzdy spravne to vlakno,
 *                ktore chceme (riesenie problemu ale je jednoduche :) )
 *	rem:	bool sleduj_close()
 *	add:	bool sleduj_close()
 *	rem:	void startCommunicateThread::run()
 *			startCommunicateThread::startCommunicateThread()
 * 15.05.2005
 *	add:	void stopCommunicateThread::run()
 *			void startCommunicateThread::run()
 *	chng:	void communicateThread::run() 
 *				- pridany cyklus pre neustalu komunikaciu
 *				- osetrenie uzavretia spojenia => recv() vracia 0
 *			void listenThread::run()
 *				- pokus o multiuser, spusta dve vlakna, jedno pre nadviazanie 
 *				  komunikacie, druhe pre ukoncenie komunikacneho vlakna
 *				- povodna verzia poskytovala moznost iba pripojenia jedneho
 *				  klienta, dovod bolo volanie comunicate->start() => nedokazalo
 *				  vytvorit viac ako jedno vlakno sucasne, cakalo do uzvretia
 * 14.05.2005
 *	add:	void listenThread::run()
 *			communicateThread::communicateThread()
 *			void communicateThread::run()
 */

#include "stdafx.h"

#include "server_thread.h"

#include "exe_cmd.h"
#include "Thread.h"
//#include "program_CLog::h"

#include <iostream> 
#include <fstream> 
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#define MAXLEN 255

//poznacim si velkost struktury sockaddr_in
int addrlen = sizeof(sockaddr_in);

CClients Clients;

/******************************************************************************
 * listenThread::listenThread(), konstruktor pre listenThread
 *****************************************************************************/
listenThread::listenThread(int p, struct in_addr addr,CClients* Clients)
: terminate(false) //, thProcess(NULL)
{
	port = p; 
	localhost = addr; 
	m_cClients=Clients;
}


/******************************************************************************
 * Implementacia listenThread::run(), navezuje komunikaciu , ktoru potom 
 * prenecha vlaknu communicateThread(),ktore spusti na kazdu poziadavku klienta
 *****************************************************************************/
void listenThread::run()
{
	//vytvorim socket pre prijem nadviazana komunikacie
	if((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) 
		== INVALID_SOCKET)
	{
		CLog::WriteLog(this, "listenThread::run()): Chyba pri vytvoreni socketu");
		return;
	}

	//pomenujem socket
	struct sockaddr_in sockName;
	sockName.sin_family = AF_INET;
	sockName.sin_port = port;
	sockName.sin_addr.s_addr = INADDR_ANY;

	if(bind(serv_sock, (sockaddr*)&sockName, addrlen)
		== SOCKET_ERROR)
	{
		CLog::WriteLog(this, "ERROR(listenThread::run()): Chyba pri pomenovani socketu, funkcia bind()!");
		closesocket(serv_sock);
		return;
	}
	CLog::WriteLog(this,"listenThread::run()): Pomenovanie socketu bolo uspesne, funkcia bind()");

	//vytvorim frontu poziadaviek na spojenie, maximalnu rozumnu :)
	if(listen(serv_sock, SOMAXCONN) == SOCKET_ERROR){
		CLog::WriteLog(this, "listenThread::run()): Chyba vo vytvarani fronty pri funkcii listen()!" );
		closesocket(serv_sock);
		return;
	}
	CLog::WriteLog(this, "listenThread::run()): Vytvorenie fronty funkciou listen bolo uspesne");

	//CShowClients ShowClients(m_cClients);

	int count = 0;
	struct sockaddr_in clientInfo;

	//prijmam spojenia
	while(!terminate){
		//prijatie spojenia, vyber z fronty poziadaviek
		SOCKET client = accept(serv_sock, (sockaddr*)&clientInfo, &addrlen);
		if(client == INVALID_SOCKET){
			if(WSAGetLastError() != WSAEINTR){
				char message[100];
				sprintf(message, "listenThread::run()): Chyba pri prijati spojenia, funkcia accept()! (%i)", WSAGetLastError());
				CLog::WriteLog(this, message);
				closesocket(serv_sock);
				return;
			}else{
				CLog::WriteLog(this,"listenThread::run()): Ukoncenie vlakna listen, socket je zavrety!");
				return;
			}
		}//if
		CLog::WriteLog(this,"listenThread::run()): Funkcia accept() bola uspesna!");

		++count; //zvysim pocet kolko sa doposial pripojilo klientov

		//prijal som spojenie, ostatne prenecham na vlakno communicateThread, 
		//ktore vlozim do vectoru
		communicateThread * communication = new communicateThread(client,clientInfo,localhost,count/*, &ShowClients*/);

		char message[100];
		sprintf(message, "listenThread::run()): Vlakno communicateThread bolo uspesne alokovane, ID: %i", count);
		CLog::WriteLog(this, message);

		//ochrana pred sucasnym pristupom do vectoru client_list inymi vlaknami
		m_cClients->Lock(this);
		m_cClients->PushClient(this, communication);
		m_cClients->UnLock(this);

		try
		{
			communication->start();
			char message[100];
			sprintf(message, "listenThread::run()): Spustenie komunikacneho vlakna bolo uspesne, ID: %i", count);
			CLog::WriteLog(this, message);
		}
		catch (ThreadException ex) 
		{
			CLog::WriteLog(this,(ex.getMessage()).c_str());
		}
	}//while()

	closesocket(serv_sock);
}//listenThread::run()

void listenThread::Process(sMessage Message)
{
	if (Message.Out && !Message.Command) {
		RunOutProcess(Message);
	} else {
		if (!Message.Command) {
			switch(Message.MessageCode) 
			{
			case END:
				closesocket(serv_sock); terminate=true;
				break;
			default:
				return;
			}
		}
	}
}


/******************************************************************************
 * konstruktor pre communicateThread
 *****************************************************************************/
communicateThread::communicateThread(SOCKET clt, struct sockaddr_in cltInfo,
             struct in_addr host, int identificator/*, CShowClients* showClients*/)
{
//	thProcess = NULL;
	socket = clt;	
  clientInfo = cltInfo;
	localhost = host;
	m_sID.type=T_INT; m_sID.t_id=identificator;
	terminate = NO;
	end = NO;
	m_bTelnet=false; m_bPutty=false;
	strcpy(telnet.terminal,"");	//ziadny TELNET client
	recvsize = 0;
//	m_ShowClients=showClients;
}//communicateThread::communicateThread()


/******************************************************************************
* void communicateThread::AnalyzeError()
*****************************************************************************/
void communicateThread::AnalyzeError()
{
	sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"Clients");
	switch (WSAGetLastError())
	{
		case WSAECONNRESET :
			CLog::WriteLog(this, "communicateThread: Prerusene spojenie mimo servra");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END);
			return;
		case WSAECONNABORTED : //vracia ak je uz socket zavrety v blokovacom rezime	
			CLog::WriteLog(this, "communicateThread: Zavrety socket, WSACONNABORTED");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END_SOCKCLOSED);	   
			return;
		case WSAENOTSOCK : //prerusenie spojenia na strane klienta
			CLog::WriteLog(this, "communicateThread: Spojenie prerusene na strane klienta");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END_SOCKCLOSED);	   
			//end = YES;
			return;
		default :
			char msg[100];
			sprintf(msg, "communicateThread: Chyba pri prijati dat, %i", WSAGetLastError());
			CLog::WriteLog(this, msg);
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END);
			return;
	}
}

/******************************************************************************
* void communicateThread::Send(char* pMsg, size_t iLen)
******************************************************************************/
void communicateThread::Send(const char* pData, int iLen)
{
	char* pMsg = new char[BUFSIZE+10];
	strcpy(pMsg, pData);
	if (m_bPutty||m_bTelnet) {
		WORD newline = CR << 8; newline |= LF; 
		strcat(pMsg, (char*)&newline);
		CLog::WriteLog(this, "communicateThread::Send(): vlozene CRLF!");
		if (end!=YES) {
			strcpy(pMsg+iLen+2, "--> "); //promt
			CLog::WriteLog(this, "communicateThread::Send(): prikopirovany prompt!");
			if(send(socket, pMsg, iLen+6, 0) == SOCKET_ERROR){
				delete [] pMsg;
				CLog::WriteLog(this,"communicateThread::Send(): Problem s odoslanim dat!");
				AnalyzeError();
			}//if()
		}else{
			if(send(socket, pMsg, iLen+2, 0) == SOCKET_ERROR){
				delete [] pMsg;
				CLog::WriteLog(this,"communicateThread::Send(): Problem s odoslanim dat!");
				AnalyzeError();
			}//if()
		}
	}else{
		if (end!=YES) {
			strcat(pMsg, "\n--> ");
			if(send(socket, pMsg, iLen+6, 0) == SOCKET_ERROR){
				delete [] pMsg;
				CLog::WriteLog(this,"communicateThread::Send(): Problem s odoslanim dat!");
				AnalyzeError();
			}
		}else{
			strcat(pMsg, "\n");
			if(send(socket, pMsg, iLen+2, 0) == SOCKET_ERROR){
				delete [] pMsg;
				CLog::WriteLog(this,"communicateThread::Send(): Problem s odoslanim dat!");
				AnalyzeError();
			}//if()
		}
	}
	CLog::WriteLog(this, "communicateThread::Send(): delete pMsg!");
	delete [] pMsg;
	CLog::WriteLog(this, "communicateThread::Send(): pMsg deleted!");
}

/******************************************************************************
* communicateThread::Process(sMessage Message)  
******************************************************************************/
void communicateThread::Process(sMessage Message)
{
	if (Message.Out && !Message.Command) {
		RunOutProcess(Message);
	} else {
		if (!Message.Command) {
			switch(Message.MessageCode) {
			case YOUARE:
				Send(Message.Data.c_str(), Message.Data.size()) ;
				break;
			default: ;
			}
		}
	}
}

/******************************************************************************
* communicateThread::Negotiation()  
*******************************************************************************/
bool communicateThread::Negotiation()
{
	if (recvsize) strcpy(telnet.terminal,"TELNET"); //default, Negotiaton s recvsize>0 sa spusti iba ked prvymi prijatymi datami bol IAC
    while (true) {
		if(((recvsize = recv(socket, message, BUFSIZE - 1, 0)) == SOCKET_ERROR)){
			return false;
		}else{
			string cas = GetTime();	//zistim cas, kedy doslo k prijatiu dat
			if (recvsize==0) {		//ked doslo k uzvretiu spojenia na strane klienta
				char msg[100];
				sprintf(msg, "communicateThread::Negotiation(): %s ; %s ; thread ID %i: Connection is closed", cas.c_str(),  inet_ntoa((in_addr)clientInfo.sin_addr), m_sID.t_id);
				CLog::WriteLog(this, msg);
				return false;
			}
			//overim ci neide o prikaz TELNETU, size overovat nemusim
			if((BYTE)message[0] == IAC){
				if (!strcmp(telnet.terminal,"")) {
					strcpy(telnet.terminal, "TELNET"); //ak je to TELNET, a este som si to nepoznacil
					WORD newline = CR << 8; newline |= LF; char msg[100];
					strcpy(msg, "Modular server 1.0:"); 
					strcat(msg, (char*)&newline);
					strcpy(msg + 21, "--> ");
					if(send(socket, msg, 25, 0) == SOCKET_ERROR){
						CLog::WriteLog(this,"communicateThread::Initialize(): Problem s odoslanim dat!");
						return false;
					}//if()
				}
				if (!telnet.telnetCommand(socket, message, recvsize)) {
					return false;
				}
			}
			else return true; //nebol to prikaz Telnetu, koniec vyjednavania
		}
	}//while (true) {
	
}

/******************************************************************************
* communicateThread::Initialize()  
*******************************************************************************/
bool communicateThread::Initialize()
{
	u_long argp;
	if (ioctlsocket(socket, FIONREAD, &argp)==SOCKET_ERROR ) {
		char msg[100];
		sprintf(msg, "communicateThread::Initialize(): Chyba na sokete: funkcia ioctlsocket()! (%i)", WSAGetLastError());
		CLog::WriteLog(this, msg);
		return false;
	}
	if (argp!=0) { //su data na sockete, ktore mozem prijat
		if(((recvsize = recv(socket, message, BUFSIZE - 1, 0)) == SOCKET_ERROR)){
			return false;
		}
		//zistim cas, kedy doslo k prijatiu dat
		string cas = GetTime();
		if (recvsize==0) { //ked doslo k uzvretiu spojenia na strane klienta
			char msg[100];
			sprintf(msg,"communicateThread::Negotiation(): %s ; %s ; thread ID %i: Connection is closed", cas.c_str(), inet_ntoa((in_addr)clientInfo.sin_addr), m_sID.t_id);
			CLog::WriteLog(this, msg);
			return false;
		}
		if ((BYTE)message[0]==IAC ) { //skontrolujem, ci to su TELNET prikazy
			WORD newline = CR << 8; newline |= LF; char msg[100];
			strcpy(msg, "Modular server 1.0:"); 
			strcat(msg, (char*)&newline);
			strcpy(msg + 21, "--> ");
			if(send(socket, msg, 25, 0) == SOCKET_ERROR){
				CLog::WriteLog(this,"communicateThread::Initialize(): Problem s odoslanim dat!");
				return false;
			}//if()
			if (!Negotiation()) return false;
			if (!strcmp(telnet.terminal, "XTERM")) {m_bPutty=true;}
			else m_bTelnet=true;
		}
	}else{
		if (!telnet.telnetCommand(socket,message,recvsize)) return false;
		if (!Negotiation()) return false;
	//	telnet.telnetCommand(socket, message, recvsize); //nepriso nic, zacnem ja navezovat spojenie
		if (!strcmp(telnet.terminal, "XTERM")) {m_bPutty=true;}
		else{
			if (strcmp(telnet.terminal, "")) {m_bTelnet=true;}
		}
	}//if(argp != 0)

	return true;
}


/******************************************************************************
* bool communicateThread::AcceptMessage(string time)  
*******************************************************************************/
bool communicateThread::AcceptMessage(string time, int& counter, char buffer[])
{
	message[recvsize] = '\0';
	//je to PUTTY alebo iny netelnetovsky klient, alebo prisiel string z Telnet klienta
	if(m_bPutty||(!m_bTelnet)||(recvsize>1 && message[0]!=CR && message[1]!=LF)){ 
		char msg[100];
		sprintf(msg, "%s  RECEIVE from %s thread ID %i bytes %i:\n%s", time.c_str(), inet_ntoa((in_addr)clientInfo.sin_addr), m_sID.t_id, recvsize, message) ;
		CLog::WriteLog(this, msg);

		vector<string> cmds = spracuj(message);
		Execute(cmds);
		counter=0;
	}else{ // echo a zapis do bufferu, co som prijal, az po CR LF
		if (recvsize==1) {
			if (counter < BUFSIZE-1) {
				*(buffer+counter) = message[0];
				++counter;
			}
			//ECHO
			if((send(socket,message,recvsize, 0)) == SOCKET_ERROR){
				CLog::WriteLog(this, "communicateThread::run(): Chyba pri posielani dat");
				AnalyzeError();
				return false;
			}
		}else{
			if (message[0]==CR && message[1]==LF) { 
				if ((send(socket,message,recvsize, 0))==SOCKET_ERROR) {	//poslem naspet novy riadok - ECHO	
					CLog::WriteLog(this, "communicateThread::run(): Chyba pri posielani dat");
					AnalyzeError();
					return false;
				}
				*(buffer+counter) = '\0';
				recvsize=sprintf(message, buffer);

				char msg[BUFSIZE+100];
				sprintf(msg, "%s  RECEIVE from %s thread ID %i bytes %i:\n%s", time.c_str(), inet_ntoa((in_addr)clientInfo.sin_addr), m_sID.t_id, recvsize, buffer) ;
				CLog::WriteLog(this, msg);

				vector<string> cmds = spracuj(message);
				CLog::WriteLog(this,"AcceptMessage(): spracuj(message) prebehlo v poriadku");
				Execute(cmds);
				CLog::WriteLog(this,"AcceptMessage(): Execute(cmds) prebehlo v poriadku");
				counter = 0;
				//CLog::WriteLog(this, "AcceptMessage(): counter=0!");
			} //if (message[0]==CR && message[1]==LF)
		}//if(recvsize == 1){
	}//if(m_bPutty||(!m_bTelnet)||...)
	CLog::WriteLog(this, "AcceptMessage(): allright!");
	return true;
}

/******************************************************************************
 * Implementacia communicateThread::run()
 * -  
 *****************************************************************************/
void communicateThread::run()
{
	//LOKALNE PREMENNE
//	int mistake, counter = 0;
	int counter=0; char buffer[BUFSIZE];
	if (!Initialize()) {
		AnalyzeError();
		return;
	}

	string time = GetTime();
	//komunikacia s klientom, prebieha dokial neprijde prikaz close 
	while(terminate!=YES){
		CLog::WriteLog(this, "communicateThread::run(): Accepting Message!");
		if ((BYTE)message[0] != IAC) { //overim ci neide o prikaz TELNETU, size overovat nemusim
			if (!AcceptMessage(time, counter, buffer)) {
				CLog::WriteLog(this, "communicateThread::run(): Message not accepted!");
				return;
			}
		}//if(message[0] != IAC)
		CLog::WriteLog(this, "communicateThread::run(): Message accepted!");
		//prijmem data od klienta
		if(((recvsize = recv(socket, message, BUFSIZE - 1, 0)) == SOCKET_ERROR)){
			AnalyzeError();
			return;
		 }
		//zistim cas, kedy doslo k prijatiu dat
		time = GetTime();
		if(recvsize == 0){ //ked doslo k uzvretiu spojenia na strane klienta
			char msg[100];
			sprintf(msg, "communicateThread::run(): %s ; %s ; thread ID %i : Connection is closed", time.c_str(),  inet_ntoa((in_addr)clientInfo.sin_addr), m_sID.t_id);
			CLog::WriteLog(this, msg);
			sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"Clients");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END);
			return;
		}
	}//while();

	CLog::WriteLog(this, "communicateThread::run(): Vlakno bude ukoncene koli terminate YES");

	sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"Clients");
	CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END);

}//communicateThread::run()*/


/******************************************************************************
 * in_addr cummunicateThread::getIP()
 *****************************************************************************/
char* communicateThread::getIP()
{
	return inet_ntoa((in_addr)clientInfo.sin_addr);
}

/******************************************************************************
* CProcCmdThread* communicateThread::FindCmdProcess(char* pName)
******************************************************************************/
CProcCmdThread* communicateThread::FindCmdProcess(char* psName)
{
	for (size_t i=0; i<vecCommandMessageThread.size(); ++i) {
		if(!strcmp(psName, (vecCommandMessageThread[i]->getName()).c_str()))
			return vecCommandMessageThread[i];
	}
	return NULL;
}

/******************************************************************************
* void  communicateThread::ProcessCommandMessage(sMessage Message)
******************************************************************************/
void  communicateThread::ProcessCommandMessage(sMessage Message)
{
	char arg[10]; _itoa_s((int)Message.MessageCode,arg,10);
	CProcCmdThread* thProcess=NULL;
	if (thProcess=FindCmdProcess(arg)){
		thProcess->Process(Message);
	}else{
		ProcCmdHandler hndProcessCmd=ProcessCmdMsg.GetHandler(arg);
		if (hndProcessCmd) 
		{
			vecCommandMessageThread.push_back(new CProcCmdThread(hndProcessCmd, arg, socket));
			//(*(vecCommandMessageThread.end()-1))->start();
			(*(vecCommandMessageThread.end()-1))->Process(Message);
		}
	}
	return;
}

/******************************************************************************
 * zavri_socket() - zavrie vsetky sockety vo vectore vlakien
 *****************************************************************************/
/*void zavri_socket()
{
    for(vector<communicateThread*>::size_type i = 0; i < client_list.size(); ++i){
		closesocket(client_list[i]->socket);
	}
}//zavri_socket()

/******************************************************************************
 * zavri_socket(id) - zavrie socket vo vectore vlakien pre vlakno s danym id
 *****************************************************************************/
/*void zavri_socket(int id)
{
	for(vector<communicateThread*>::size_type i = 0; i < client_list.size();++i)
	{
			if(client_list[i]->id == id)
                closesocket(client_list[i]->socket);
	}
}//zavri_socket(id)


/*****************************************************************************
 * ***************		funkcie pre pracu s CClients		**************** *
 * ***************				  BEGIN						**************** *
 *****************************************************************************/
bool CClients::PushClient(void* Who, communicateThread* client)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			client_list.push_back(client);
			return true;
		}
	}
	return false;
}

size_t	CClients::GetSize(void* Who)
{  
	if(m_bLocked){
		if (m_idLocker==Who) { 
			return client_list.size();
		}
	}
	return -2; 
}

int CClients::GetClientId(void* Who, index i)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			return client_list[i]->m_sID.t_id;
		}
	}
	return -1;
}

SOCKET CClients::GetClientSocket(void* Who, index i)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			return client_list[i]->socket;
		}
	}
	return NULL;
}

char* CClients::GetClientIP(void* Who, index i)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			return client_list[i]->getIP();
		}
	}
	return NULL;
}
CODE CClients::GetClientEnd(void* Who, index i)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			return client_list[i]->end;
		}
	}
	return OTHER;
}
CODE CClients::GetClientTerminate(void* Who, index i)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			return client_list[i]->terminate;
		}
	}
	return OTHER;
}

bool CClients::SetClientEnd(void* Who, index i, bool b)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			if(b) client_list[i]->end = YES;
			else  client_list[i]->end = NO;
			return true;
		}
	}
	return false;
}

bool CClients::SetClientTerminate(void* Who, index i, bool b)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			if(b) client_list[i]->terminate = YES;
			else  client_list[i]->terminate = NO;
			return true;
		}
	}
	return false;
}

void CClients::ClientEnd(void * Who, index i, bool bCloseSocket)
{
	if(m_bLocked && m_idLocker==Who){
		if (i>=0) {
			int id = client_list[i]->m_sID.t_id;
			if(!bCloseSocket) closesocket(client_list[i]->socket);
			client_list[i]->StopCommandsThread();
			client_list[i]->StopCommandProcessMessageThread();
			client_list[i]->stop(); 		
			delete client_list[i]; //bol dynamicky alokovany
			client_list.erase(client_list.begin() + i);
			char msg[100];
			sprintf(msg, "CClients::ClientEnd : Vlakno %i uspesne zastavene a vypustene z fronty", id);
			CLog::WriteLog(this, msg);
		}
	}
}

index CClients::GetIndex(void* Who, int id)
{
	if(m_bLocked){
		if (m_idLocker==Who) {
			for (size_t i = 0; i < client_list.size(); ++i) {
				if (client_list[i]->m_sID.t_id==id) {
					return i;
				}
			}
			return -1;
		}
	}
	return -2;
}

bool CClients::IsClientTelnet(void* Who, index i)
{
	if(m_bLocked && m_idLocker==Who){
		if(client_list[i]->m_bTelnet || client_list[i]->m_bPutty) return true;
		else return false;
	}
	return false;
}

void CClients::Process(sMessage Message)
{
	string YouAre;
	if (Message.Out && !Message.Command) {
		RunOutProcess(Message);
	} else {
		if (Message.Command) 
		{
			Lock(this);
			for (size_t i=0; i<client_list.size(); ++i) {
				client_list[i]->ProcessCommandMessage(Message);
			}
			UnLock(this);
		} 
		else 
		{
			switch(Message.MessageCode) 
			{
			case END:
				{
					if (Message.nameReceiver.type==T_PCHAR && !strcmp(Message.nameReceiver.t_name,"ALL")) {
						Lock(this);
						char sprava[100]; int iLen=0;
						if(Message.nameSender.type==T_INT){
							iLen=sprintf(sprava, "Server is closing! Command close from: %s client ID %i", GetClientIP(this, GetIndex(this, Message.nameSender.t_id)), Message.nameSender.t_id);
						}
						while (client_list.size()>0) {
							if(iLen) client_list[0]->Send(sprava, iLen);
							ClientEnd(this, 0);
						}
						UnLock(this);
					}else{
						Lock(this);
						ClientEnd(this, GetIndex(this,Message.nameSender.t_id));
						UnLock(this);
					}
					break;
				}
			case END_SOCKCLOSED:
				Lock(this);
				ClientEnd(this, GetIndex(this,Message.nameSender.t_id), true);
				UnLock(this);
				break;
			case WHO:
				YouAre=FillInfoWho(Message.nameSender);
				CMessageQueqe::PushBackMessageToQueqe(m_sId, Message.nameSender, YOUARE, YouAre.c_str());
				break;
			default: 
				return;
			}
		}
	}
}


bool CClients::Process(void* Who, index i, sMessage Message)
{
	if(m_bLocked && m_idLocker==Who){
		if (i>=0) {
			if(Message.Command){
				client_list[i]->ProcessCommandMessage(Message);
				return true;
			}else{			
				client_list[i]->Process(Message);
				return true;
			}
		}
	}
	return false;
}

string CClients::FillInfoWho(sID ID)
{
	std::string youare;
	Lock(this);
	if (IsClientTelnet(this, GetIndex(this, ID.t_id))) {
		for (size_t i=0; i<client_list.size(); ++i) {
			if (i<client_list.size()-1) {
				char iam[100];
				int iLen=sprintf(iam, "%s\t, client ID: %i%", client_list[i]->getIP(), client_list[i]->m_sID.t_id,CR,LF);
				WORD newline = CR << 8; newline |= LF;
				strcat(iam, (char*)&newline);
				strcpy(iam+iLen+2, "\0");
				youare.append(iam);
			}else{
				char iam[100];
				sprintf(iam, "%s\t, client ID: %i", client_list[i]->getIP(), client_list[i]->m_sID.t_id);
				youare.append(iam);
			}
		}
		UnLock(this);
		return youare;
	}else{
		for (size_t i=0; i<client_list.size(); ++i) {
			if (i<client_list.size()-1) {
				char iam[100];
				sprintf(iam, "%s\t, client ID: %i%\n", client_list[i]->getIP(), client_list[i]->m_sID.t_id);
				youare.append(iam);
			}else{
				char iam[100];
				sprintf(iam, "%s\t, client ID: %i", client_list[i]->getIP(), client_list[i]->m_sID.t_id);
				youare.append(iam);
			}
		}
		UnLock(this);
		return youare;
	}
}
/*****************************************************************************
* ***************		funkcie pre pracu s CClients		**************** *
* ***************				  END						**************** *
*****************************************************************************/
/*

int CClients::GetClientsID(int* clients)
{
	int * aClients=NULL; int iSize=0;
	m_Clients->Lock(this);
	aClients= new int [m_Clients->GetSize(this)+1];
	if (aClients) {
		for (index i=0; i<m_Clients->GetSize(this); ++i) {
			aClients[i]=m_Clients->GetClientId(this, i);
		}
		iSize=m_Clients->GetSize(this); 
	}
	m_Clients->UnLock(this);
	clients=aClients;
	return iSize;	
}//*/