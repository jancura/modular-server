//definuje funkcie pre spracovanie IP adries, konverziu integera na string
//a obratenie DWORDU

/*Pali Jancura - to jest ja :)
 * 08.05.2005
 *	chng:	inicializujGenerovanie() - osetrenie pre masku 0.0.0.0
 * 07.05.2005
 *	rem:	my_ping() - presunuty do ping.cpp
 *			checksum() - presunuty do ping.cpp
 * 06.05.2005 - a ideme na samotny ping
 *	add:	my_ping() - nedokoncene
 *			checksum() - ma nejaku chybu pri pointri => pozmeneby original
 * 29.04.2005 - zaciatok komentovania, dufam, ze konecne rozumna verzia generovania IP adries	
 *	add:	globalne premenne lastIP; pocetIP; subnetCh[33];
 *			getLocalIP()			
 *			networkAddress()
 *			spracujSubnet()
 *			generujIP()
 *			inicializujGenerovanie()
 *			obratDWORD()
 *			dectostr()
 */	 

#include "stdafx.h"

#include "spracuj_net_addr.h"

#include <stdexcept>
#include <winsock.h>
#include <iostream>

#include "ip_icmp.h"
#include "iphdr.h"

//maximalna dlzka pre name loclahostu a velkosti bufferu
#define maxlen 255 

//definovanie globalnych premennych
static struct in_addr lastIP;
static int pocetIP;
static unsigned char subnetCh[32];  //32 pretoze nebudem inicializovat pre 
                                    //masku 0.0.0.0


/******************************************************************************
 * getLocalIP() - v l-e byteorder
 *****************************************************************************/
int getLocalIP(struct in_addr *in)
{
	char hostname[maxlen];

	try{
        if(gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
			throw std::domain_error("chyba v getLocalIP () pri gethostname()");
		
		hostent *H = gethostbyname(hostname);

		if(H == NULL)
			throw std::domain_error("chyba v getLocalIP() gethostBYname()");

		if(H->h_addr_list[0] == NULL)
			throw std::domain_error("chyba v getLocalIP() - H->h_addr_list je prazdny");

		*in = *(in_addr *)H->h_addr_list[0];
		return 0;
	}catch (std::domain_error e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
} //getLocalIP() */


/******************************************************************************
 * networkAddres - ku danej ip adrese priradi na zaklade subnet masky adresu 
 * siete 
 *****************************************************************************/
struct in_addr networkAddress(struct in_addr addr, struct in_addr subnet)
{
	struct in_addr naddr;
	naddr.s_addr = addr.s_addr & subnet.s_addr;
	return naddr;
} //networkAddres  */


/******************************************************************************
 * spracujSubnet - vrati pocet nul v subnet maske a naplni pole s poziciami
 * tychto nul (znak 32 je ukoncovaci znak) 
 *****************************************************************************/
int spracujSubnet(struct in_addr subnet)
{
	int S_addr = (subnet.s_addr);
	int pocet = 0;
	for(int i = 31; i >= 0; --i){
		if (S_addr >> i) S_addr -= 1 << i;
		else subnetCh[pocet++] = i;			
	}
	subnetCh[pocet] = 32;  
	if(pocet == 0) return -1;
	return pocet;
}//spracujSubnet() */


/******************************************************************************
 * vygeneruje IP adresu na zaklade predchadzajucej IP ulozenej v globalne 
 * premennej lastIP
 *****************************************************************************/
int generujIP(struct in_addr* in)
{
	int i = 0, pom = 255 << 24;	

	if(pocetIP == 0) return -1;
	//generuje novu IP podla lastIP pomocou binarnej operacie xor a uklada ju hned do lastIP 
	while(subnetCh[i] != 32){
		lastIP.s_addr = lastIP.s_addr^(1 << subnetCh[i]);
		if (lastIP.s_addr & (1<<subnetCh[i++])) break; 
	}
	pocetIP--;
	//overi, ci neide o niektoru z specialnych IP vyhradenych pre siet
	if( ((lastIP.s_addr & pom) == 0) || ((lastIP.s_addr & pom) == pom) ){
	       if (generujIP(in)) return -1;
	}else *in = lastIP; //ak nie priradi vysledok, teda novu IP

	return 0;
} //generujIP() */


/******************************************************************************
 * inicializuje hodnoty pre funkciu generujIP()
 *****************************************************************************/
int inicializujGenerovanie(struct in_addr localIP, struct in_addr subnet)
{
	int i;
	if(!(subnet.s_addr == 0)){
        lastIP = networkAddress(localIP, subnet);
		if((i = spracujSubnet(subnet)) == -1) return -1;
		pocetIP = (1 << i);
		return 0;
	}//je sprosta maska,pre 1<<32 nestaci typ int, preto to nebudem podporovat :)
	return -1;
}//inicializujGenerovanie() */
