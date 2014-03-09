//implemetacia  Queqe.h


#include "stdafx.h"
#include <string>

#include "Queqe.h"	

bool CMessageQueqe::PopMessage(sMessage& Message) 
{ 
	Lock(this);
	if (lMessages.size() > 0) {
		Message = *lMessages.begin(); 
		lMessages.pop_front();
		UnLock(this);
		return true; 
	}
	UnLock(this);
	return false; 
}//*/


void PushBackMessageToQueqeOut(char* Sender, char* Receiver, DWORD Code, const char* pData)
{
	if (Code>0){
		sID IDSender; sID IDReceiver;
		if (strlen(Sender)+1<=20) {
			IDSender.type=T_PCHAR;
			strcpy(IDSender.t_name, Sender);
		}else return;
		if (Receiver){
			if(atoi(Receiver)>0) {
				IDReceiver.type=T_INT;
				IDReceiver.t_id=atoi(Receiver);
			} else {
				if (strlen(Receiver)+1<=20) {
					IDReceiver.type=T_PCHAR;
					strcpy(IDReceiver.t_name, Receiver);
				}else return;
			}
		} else return;
		CMessageQueqe::PushBackMessageToQueqe(IDSender, IDReceiver, Code, pData, TRUE, FALSE);
	}
}

void PushBackCommandToQueqeOut(char* Sender, char* Receiver, DWORD Code, const char* pData)
{
	if (Code>0){
		sID IDSender; sID IDReceiver;
		if (strlen(Sender)+1<=20) {
			IDSender.type=T_PCHAR;
			strcpy(IDSender.t_name, Sender);
		}else return;
		if (Receiver){
			if(atoi(Receiver)>0) {
				IDReceiver.type=T_INT;
				IDReceiver.t_id=atoi(Receiver);
			} else {
				if (strlen(Receiver)+1<=20) {
					IDReceiver.type=T_PCHAR;
					strcpy(IDReceiver.t_name, Receiver);
				}else return;
			}
		} else return;
		CMessageQueqe::PushBackMessageToQueqe(IDSender, IDReceiver, Code, pData, TRUE, TRUE);
	}
}


void CMessageQueqe::PushBackMessageToQueqe(sID Sender, sID Receiver, DWORD Code, const char* pData, bool bOut, bool bCmd)
{
	sMessage Message;
	Message.nameSender   = Sender;
	Message.nameReceiver = Receiver;
	Message.MessageCode  = Code;
	if(pData) {
		std::string s(pData); Message.Data=s;}
	Message.Out=bOut;
	Message.Command= bCmd;
	CMessageQueqe::GetInstance()->PushMessage(Message);
}

CProcessQueqe::CProcessQueqe(recvBroadcastThread* RecvBroad, sendBroadcastThread* SendBroad, 
							 listenThread* Listen, CClients* Clients)
{
	m_cClients		= Clients;
	m_thRecvBroad	= RecvBroad;
	m_thSendBroad	= SendBroad;
	m_thListen		= Listen;
}

void CProcessQueqe::Process()
{
	while(true){
		if (CMessageQueqe::GetInstance()->PopMessage(Message)) {
			if(Message.nameReceiver.type == T_PCHAR)
			{
				if (!strcmp(Message.nameReceiver.t_name,"recvBroadcast")) {
					if (m_thRecvBroad) m_thRecvBroad->Process(Message);
					continue;
				}
				if (!strcmp(Message.nameReceiver.t_name,"sendBroadcast")) {
					if (m_thSendBroad) m_thSendBroad->Process(Message);
					continue;
				}
				if (!strcmp(Message.nameReceiver.t_name,"Listen")) {
					m_thListen->Process(Message);
					continue;
				}
				if (!strcmp(Message.nameReceiver.t_name,"Clients")) {
					m_cClients->Process(Message);
					continue;
				}
				if (!strcmp(Message.nameReceiver.t_name, "ALL")) 
				{
					if (m_thRecvBroad) m_thRecvBroad->Process(Message);
					if (m_thSendBroad) m_thSendBroad->Process(Message);
					m_thListen->Process(Message);
					m_cClients->Process(Message);
					if (Message.Out && !Message.Command) {
						RunOutProcess(Message);
					}
					if (Message.MessageCode==END) { 
						RunOutProcess(Message);
						return;
					}
					continue;
				}

				if (Message.Out && !Message.Command) {
					RunOutProcess(Message);
					continue;
				}
			}
			else
			{
				if (Message.nameReceiver.type==T_INT) {
					m_cClients->Lock(this);
					for (size_t i=0; i<m_cClients->GetSize(this); ++i) {
						if(Message.nameReceiver.t_id==m_cClients->GetClientId(this,i)) {
							m_cClients->Process(this, i, Message);
							break;
						}
					}
					m_cClients->UnLock(this);
				}
			}//if(Message.nameReceiver.type == T_PCHAR)
		}else Sleep(TIMER);  //if (CMessageQueqe::GetInstance()->PopMessage(Message))
	}
}


