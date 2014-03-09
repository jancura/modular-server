
#include "stdafx.h"

#include "ChatThread.h"

#include <WinSock2.h>
#include <vector>

void Listen()
{
	int addrlen=sizeof(sockaddr_in);
	SOCKET SockChat;
	//vytvorim socket pre prijem nadviazana komunikacie
	if((SockChat = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))	== INVALID_SOCKET){
		return;
	}

	//pomenujem socket
	struct sockaddr_in sockName;
	sockName.sin_family = AF_INET;
	sockName.sin_port = htons(9999);
	sockName.sin_addr.s_addr = INADDR_ANY;

	if(bind(SockChat,(sockaddr*)&sockName,addrlen)==SOCKET_ERROR)
	{
		closesocket(SockChat);
		return;
	}

	//vytvorim frontu poziadaviek na spojenie, maximalnu rozumnu :)
	if(listen(SockChat, SOMAXCONN) == SOCKET_ERROR){
		closesocket(SockChat);
		return;
	}

	struct sockaddr_in clientInfo;
	int ID=0;

	//prijmam spojenia
	while(true){
		//prijatie spojenia, vyber z fronty poziadaviek
		SOCKET sock_client = accept(SockChat, (sockaddr*)&clientInfo, &addrlen);
		if(sock_client==INVALID_SOCKET){
			if(WSAGetLastError() != WSAEINTR){//Chyba pri prijati spojenia, funkcia accept()!
				closesocket(SockChat);
				return;
			}else{//Ukoncenie vlakna listen, socket je zavrety!"
					return;
			}
		}//if
		
		++ID;
		//prijal som spojenie, ostatne prenecham na vlakno communicateThread, ktore vlozim do vectoru
		CClientThread * ClientThread= new CClientThread(sock_client,ID,clientInfo);

		ClientList.Lock("Listen");
		ClientList.Push("Listen", ClientThread);
		ClientList.UnLock("Listen");

		try
		{
			ClientThread->start();
			//ClientThread->stop();
		}
			catch (ThreadException ex) {;}

	}//while()
	closesocket(SockChat);
}

//**********************************************************************************************//

void CClientThread::run()
{
	int counter=0; char message[BUFSIZE]; int iRecvSize=0;
	//komunikacia s klientom, prebieha dokial neprijde prikaz close 
	while(!m_bTerminate){
		if(((iRecvSize=recv(m_Socket, message, BUFSIZE - 1, 0)) == SOCKET_ERROR)){
			switch (WSAGetLastError())
			{
				case WSAECONNABORTED : //vracia ak je uz socket zavrety v blokovacom rezime	 
				case WSAENOTSOCK : //prerusenie spojenia na strane klienta
					break;
				default :
					closesocket(m_Socket);
			}
			m_bTerminate=true;
			return;
		}//if

		if(iRecvSize==0) {	//ked doslo k uzvretiu spojenia na strane klienta
			m_bTerminate=true; return;
		}
		message[iRecvSize]='\0';
		ClientList.Lock(this);
		ClientList.Send(this, message, m_iID);
		ClientList.UnLock(this);

	}//while();
	//m_bTerminate=true;
}

