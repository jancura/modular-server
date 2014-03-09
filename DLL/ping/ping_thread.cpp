//13.05.2005 - zdrojovy subor pre implemetaciu vlakien pracujucich so socketmi 
//pre ICMP ping

/*Pali Jancura - to jest ja :)
 * 13.05.2005
 *	add:	unsigned short checksum(unsigned char *addr, int count);
 *			posliICMP::posliICMP(DWORD localIP, DWORD subNetMask)
 *			void posliICMP::run()
 *			prijmiICMP::prijmiICMP(vector<in_addr>& zoznamIP)
 *			void prijmiICMP::run()
 */

#include "stdafx.h"

#include "ping_thread.h"

#include "spracuj_net_addr.h"
#include "ip_icmp.h"
#include "iphdr.h"

#include <vector>
#include <winsock.h>
#include <string>
#include <fstream>

using namespace std;

#include "ou_thread.h"
using namespace openutils;

#define BUFSIZE 1000


/******************************************************************************
 * funkcia checksum je opisana z prislusnych RFC dokumentov
 * sluzi pre spocitanie crc - kontrolneho suctu paketu
 *****************************************************************************/
unsigned short checksum(unsigned char *addr, int count)
{
  register long sum = 0;
  unsigned short * s = (unsigned short *)addr;

  while( count > 1 )  
  {
    /*  This is the inner loop */
    sum += * s++;
    count -= 2;
  }  
  /*  Add left-over byte, if any */
  if( count > 0 )
    sum += * (unsigned char *) s;
  /*  Fold 32-bit sum to 16 bits */
  while (sum>>16)
    sum = (sum & 0xffff) + (sum >> 16);

  return ~sum;
}//checksum()*/


/******************************************************************************
 * implemtacia konstruktoru posliICMP::posliICMP()
 *****************************************************************************/
posliICMP::posliICMP(struct in_addr localIP, struct in_addr subNetMask,
					 ofstream& l)
{
	locIp = localIP;
	subNet = subNetMask;
	log = &l;
}// posliICMP::posliICMP() */


/******************************************************************************
 * implementacia poslielania ICMP paketov
 *****************************************************************************/
void posliICMP::run()
{
	//lokalne premenne pre pracu so socketmi
	SOCKET sock;
	unsigned int ttl;
	//string vypis;
	
	//vytvorenie socketu
	if((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET){
		*log << "CHYBA PRI VYTVARANI SOCKETU!" << endl;
	}else{
		//TTL
		ttl = 255;
		setsockopt(sock, IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl));
				
		//inicializacia generovania
		struct in_addr addr;
		if(inicializujGenerovanie(locIp, subNet)){
			*log << "CHYBA PRI INICIALIZACII!" << endl;
			closesocket(sock);
		}else{ //generovanie a zaslanie ICMP paketu na vygenerovanu adresu
            int pocet = 0; 
			sockaddr_in sendSockAddr;
			while(!generujIP(&addr))
			{
  				pocet++; 
				if(addr.s_addr != locIp.s_addr/*true*/){
					icmphdr *icmp;
					//nastavenie podla prave beziaceho procesu, koli jednoznacnosti
					DWORD pid = GetCurrentProcessId();// + pocet;
					
					//vyplnime icmp hlavicku
					icmp = new icmphdr;
					icmp->code = 0;
					icmp->type = ICMP_ECHO;
					icmp->un.echo.id = pid;
					icmp->checksum = 0;
					icmp->un.echo.sequence = 1;
					icmp->checksum = checksum((unsigned char *)icmp, sizeof(icmphdr));
					//pripravime sendto()
					sendSockAddr.sin_family = AF_INET;
					sendSockAddr.sin_port = 0;
					sendSockAddr.sin_addr = addr;

					sendto(sock, (char *)icmp, sizeof(icmphdr), 0, 
						  (sockaddr *)&sendSockAddr, sizeof(sockaddr));
					
					delete icmp;
				}
			}
			closesocket(sock);
		}
	}
	return;
}//posliICMP::run() */


/******************************************************************************
 * konstruktor pre vlakno prijmiICMP
 *****************************************************************************/
prijmiICMP::prijmiICMP(vector<string>& zoznamIP, DWORD t, ofstream& l)
{
	zoznam = &zoznamIP;
	timeOut = t;
	log = &l;
}// prijmiICMP::prijmiICMP */


/******************************************************************************
 * implementacia prijmania ICMP paketov vo vlakne prijmiICMP
 * vytvori si pre to vlastny socket, pri odchyteni ulozi info o IP do vectoru
 *****************************************************************************/
void prijmiICMP::run()
{
	//lokalne premenne pre pracu so socketmi
	SOCKET sock;
	sockaddr_in sockName;

	//vytvorenie socketu
	if((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET){
		*log << "CHYBA PRI VYTVARANI SOCKETU!" << endl;
		return;
	}
	//nastavenie neblokovacieho rezimu
	u_long arg = 1;
	// Operace je FIONBIO. Parametr je ukazatel na nenulové èíslo.
	if (ioctlsocket(sock, FIONBIO, &arg) == SOCKET_ERROR)
	{
		*log << "CHYBA PRI PREPNUTI DO NEBLOKOVACIEHO REZIMU!" << endl;
		return;
	}
	//pomenovanie socketu
	sockName.sin_family = AF_INET;
	sockName.sin_port = 0;
	sockName.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (sockaddr *)&sockName, sizeof(sockName)) == -1)
	{
		*log << "Problem s pojmenovanim soketu!" << endl;
		closesocket(sock);
		return;
	}

	//lokalne premenne
	DWORD time;
	icmphdr *icmpRecv;
	iphdr *ip;
	int lenght;
	char buffer[BUFSIZE];
	DWORD pid = GetCurrentProcessId();//nastavenie podla prave beziaceho procesu, koli jednoznacnosti
	sockaddr_in receiveSockAddr;
	int size = sizeof(sockaddr_in);
	
	//koli prepoctu cakacej doby pingovania
	time = GetTickCount();
	while(time + timeOut > GetTickCount()){			
		while((lenght = recvfrom(sock, buffer, BUFSIZE, 0,
		(sockaddr *)&receiveSockAddr, &size)) != SOCKET_ERROR ){									
			//Prisel blok dat IP hlavicka + ICMP hlavicka.  
			ip = (iphdr *)buffer;
			icmpRecv = (icmphdr *)(buffer + ip->ihl * 4);
			if((icmpRecv->un.echo.id == pid) && (icmpRecv->type == ICMP_ECHOREPLY)){
				zoznam->push_back(inet_ntoa(receiveSockAddr.sin_addr));
			}
			//else zoznam->push_back("iny ICMP_PACKET");
		}
		if(WSAGetLastError() != WSAEWOULDBLOCK){
			*log << "Chyba pri recvfrom() ICMP paketov: " 
				<< WSAGetLastError() << endl;
			//chyba
		}
  	}
	closesocket(sock);
}//prijmiICMP::run() */
