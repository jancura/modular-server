//implemetacia int GetClientsID(int* clients)

#include "stdafx.h"

#include "ShowIDClients.h"
#include "server_thread.h"

int GetClientsID(int* clients)
{
	int * aClients=NULL; int iSize=0;
	Clients.Lock("ShowIDClients");
	aClients= new int [Clients.GetSize("ShowIDClients")+1];
	if (aClients) {
		for (index i=0; i<Clients.GetSize("ShowIDClients"); ++i) {
			aClients[i]=Clients.GetClientId("ShowIDClients", i);
		}
		iSize=Clients.GetSize("ShowIDClients"); 
	}
	Clients.UnLock("ShowIDClients");
	clients=aClients;
	return iSize;	
}
/*
SOCKET GetSocket(index i)
{
	SOCKET sock;
	Clients.Lock("GetSocket");
	sock = Clients.GetClientSocket("GetSocket", i)
	Clients.UnLock("GetSocket");
	return sock;
}//*/
