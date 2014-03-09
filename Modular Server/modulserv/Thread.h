// deklaracia a definicie dalsich threadov pre podporu funkcii z dll

#ifndef MY_THREAD_H
#define MY_THREAD_H

//#include "StdAfx.h"
#include "ShowIDClients.h"
#include "my_ou_thread.h"
//#include "ou_thread.h"
#include "Plugins.h"
#include "Message.h"
#include "Queqe.h"


using namespace my_openutils;
//using namespace openutils;

class CProcessThread : public MyThread {
public:
	CProcessThread(ProcessHandler Process, const char* ProcessName) { fProcess=Process; setName(ProcessName);};
	inline void run() 
	{
		while (true) {
			if (m_bProcess) {
				m_bProcess=false;
				char* Sender = new char[strlen(Message.nameSender.t_name)+1];
				int iLenSend = sprintf(Sender, Message.nameSender.t_name);
				char* Receiver=NULL; int iLenRecv=0;
				if (Message.nameReceiver.type==T_PCHAR) {
					Receiver=new char[strlen(Message.nameReceiver.t_name)+1];
					iLenRecv = sprintf(Receiver, Message.nameReceiver.t_name);
				}else{
					if (Message.nameReceiver.type==T_INT) {
						char arg[10];
						_itoa_s(Message.nameReceiver.t_id,arg,10) ;
						Receiver=new char[strlen(arg)+1];
						iLenRecv = sprintf(Receiver, arg);
					}
				}
				char* Data=NULL; int iLenData=0;
        if (Message.Data.size()>0) {
					Data = new char[Message.Data.size()];
					iLenData = sprintf(Data, Message.Data.c_str());
				}
				fProcess(Sender, iLenSend, Receiver, iLenRecv, Message.MessageCode, Data, iLenData);
				sleep(20);
				if (Sender) delete [] Sender;
				if (Receiver) delete [] Receiver;
				if (Data) delete [] Data;
			}
		}
	};
	inline void Process(sMessage Msg)
	{
		m_bProcess=true; 
		Message=Msg;
		resume(); 
		sleep(100); //100ms na vykonanie kodu, potom nastane suspend
		suspend1();
	};
public:
	bool m_bProcess;
private:
	ProcessHandler fProcess;
	sMessage Message;
};


CProcessThread* FindProcess(const char* pName, const std::vector<CProcessThread*>& vecProcesses);
void RunOutProcess(sMessage Message);
void StopProcesses();



class CProcCmdThread : public MyThread {
public:
	CProcCmdThread(ProcCmdHandler Process, const char* ProcessName, SOCKET sock/*, int Id*/) 
	: m_bProcess(false) { fProcess=Process; setName(ProcessName); m_Socket=sock;};

	void run() 
	{
		//while (true) {
		//	if (m_bProcess) {
		//		m_bProcess=false;
		char* Sender = new char[strlen(Message.nameSender.t_name)+1];
		sprintf(Sender, Message.nameSender.t_name);
		char* Receiver=NULL; 
		if (Message.nameReceiver.type==T_PCHAR) {
			Receiver=new char[strlen(Message.nameReceiver.t_name)+1];
			sprintf(Receiver, Message.nameReceiver.t_name);
		}else{
			if (Message.nameReceiver.type==T_INT) {
				char arg[10];
				_itoa_s(Message.nameReceiver.t_id,arg,10) ;
				Receiver=new char[strlen(arg)+1];
				sprintf(Receiver, arg);
			}
		}

		int * clients=NULL;
		int iSize= ::GetClientsID(clients);
		fProcess(Sender, Receiver, clients, iSize, m_Socket, Message.MessageCode, Message.Data.c_str());
		
		//sleep(20);
		if (clients) delete [] clients; //GetClientsID ho alokuje dynamicky ako pole*/
		if (Sender) delete [] Sender;
		if (Receiver) delete [] Receiver;
		//if (Data) delete [] Data;
			//}
		//}
	};
	inline void Process(sMessage Msg)
	{
		if (!m_bProcess) {
			m_bProcess=true;
			Message=Msg;
			start();	
			sleep(TIMER);
			stop();
		} 
		m_bProcess=false;
	/*	m_bProcess=true; 
		Message=Msg;
		resume(); 
		sleep(100); //100ms na vykonanie kodu, potom nastane suspend
		suspend1();//*/
	};
public:
	inline bool IsRunning() {return m_bProcess;};
	
private:
	ProcCmdHandler fProcess;
	sMessage Message;
	bool m_bProcess;
	//int m_Id;
	SOCKET m_Socket;
};


class CCommandThread : public MyThread {
public:
	CCommandThread(CommandHandler Command, const char* CommandName, tPushMsgHandler PushMessageHandler, SOCKET sock, int Id) 
	: m_Id(Id), m_bRunCommand(false) 
	{ 
		fCommand=Command; setName(CommandName); PushMessage=PushMessageHandler; socket=sock;
		m_pParameters=NULL;
	};

	~CCommandThread() {if (m_pParameters) delete [] m_pParameters;};
	inline void run() 
	{
		int * clients=NULL;
		int iSize= ::GetClientsID(clients);
		fCommand(clients, iSize, socket, m_Id, PushMessage, m_pParameters);
		CLog::WriteLog(this, "CCommandThread::run(): Prikaz uspesne prebehol!");
		if (clients) delete [] clients; //GetClientsID ho alokuje dynamicky ako pole*/
		if (m_pParameters) delete [] m_pParameters; m_pParameters=NULL;		
	};
	inline void RunCommand(const char * parameters=NULL)
	{
		if (!m_bRunCommand) {
			m_bRunCommand=true;
			if (parameters) {
				m_pParameters=new char[strlen(parameters)+1];
				strcpy(m_pParameters,parameters);
			} else m_pParameters=NULL;
			start();	
			stop2();
		} 
		m_bRunCommand=false;
	};

public:
	inline bool IsRuning() {return m_bRunCommand;};

private:
	bool  m_bRunCommand;
	CommandHandler fCommand;
	tPushMsgHandler PushMessage;
	char * m_pParameters;
	SOCKET socket;
	int		m_Id;
};

CCommandThread* FindCommandThread(const char* pName, const std::vector<CCommandThread*>& vecCommand);
void RunOutCommand(const char * Command, CommandHandler hndCommand, SOCKET socket, int Id);
//void StopCommandsThread();



class CFunctionThread : public MyThread {
public:
	CFunctionThread(FunctionHandler hndFunction) {fFunction=hndFunction;};
	inline void run()
	{
		fFunction();
	};
private:
	FunctionHandler fFunction;
};



class CSendingFunctionThread :public MyThread {
public:
	CSendingFunctionThread(SendMsgHandler hndSendingFunction, tPushMsgHandler PushMessageHandler) 
	{fSendingFunction=hndSendingFunction;Push=PushMessageHandler;};
	inline void run()
	{
		fSendingFunction(Push);
		//sizeof(Push);
	};
private:
	SendMsgHandler fSendingFunction;
	tPushMsgHandler Push;
	
};


#endif