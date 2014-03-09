//implemetacia funkcii pre pracu s vectorami vlakien CProcessThread a CCommanndThread

#include "StdAfx.h"

#include "Thread.h"
#include "server_thread.h"
#include "my_parser.h"

/*
 *	
 */

std::vector<CProcessThread*>  vecProcessThread;

CProcessThread* FindProcess(const char* psName, const vector<CProcessThread*>& vecProcesses)
{
	//char arg[10];
	for (size_t i=0; i<vecProcesses.size(); ++i) {
		if(!strcmp(psName, (vecProcesses[i]->getName()).c_str()))
			return vecProcesses[i];
	}
	return NULL;
}


void RunOutProcess(sMessage Message)
{
	if (!strcmp(Message.nameReceiver.t_name, "ALL")) {
		for (size_t i=0; i<vecProcessThread.size(); ++i) {
			vecProcessThread[i]->Process(Message);
		}
		return;
	}
	CProcessThread* thProcess=NULL;
	if (thProcess=FindProcess(Message.nameReceiver.t_name, vecProcessThread)){
		thProcess->Process(Message);
	}else{
		ProcessHandler hndProcess=Processes.GetHandler(Message.nameReceiver.t_name);
		if (hndProcess) 
		{
			vecProcessThread.push_back(new CProcessThread(hndProcess, Message.nameReceiver.t_name));
			(*(vecProcessThread.end()-1))->start();
			(*(vecProcessThread.end()-1))->Process(Message);
		}
	}
}


void StopProcesses()
{
	while (vecProcessThread.size()>0) {
		vecProcessThread[0]->resume();
		vecProcessThread[0]->stop();
		delete vecProcessThread[0];
		vecProcessThread.erase(vecProcessThread.begin());
	}
}

///////////////////////////////////////////////////////////////////////////////

/*
 *	
 */



CCommandThread* FindCommandThread(const char* psName, const vector<CCommandThread*>& vecCommand)
{
	for (size_t i=0; i<vecCommand.size(); ++i) {
		if(!strcmp(psName, (vecCommand[i]->getName()).c_str()))
			return vecCommand[i];
	}
	return NULL;
}

void communicateThread::RunOutCommand(const char * Command, CommandHandler hndCommand)
{
	CCommandThread * thCommand=NULL;
	size_t i=0; string sName = word(Command, i);
	if (thCommand=FindCommandThread(sName.c_str(), vecCommandThread)){
		if (strlen(Command)>i) 
			thCommand->RunCommand(Command+i);
		else
			thCommand->RunCommand();
	}else{
		if (hndCommand) 
		{
			vecCommandThread.push_back(new CCommandThread(hndCommand, sName.c_str(), PushBackCommandToQueqeOut, socket, m_sID.t_id));
			//(*(vecCommandThread.end()-1))->start();
			if (strlen(Command)>i) {
				(*(vecCommandThread.end()-1))->RunCommand(Command+i);
			}else {
				(*(vecCommandThread.end()-1))->RunCommand();
			}
		}
	}
}


void communicateThread::StopCommandsThread()
{
	while (vecCommandThread.size()>0) {
		if (vecCommandThread[0]->IsRuning()) {
			vecCommandThread[0]->stop();
		}
		delete vecCommandThread[0];
		vecCommandThread.erase(vecCommandThread.begin());
	}
}