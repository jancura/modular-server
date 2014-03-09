//20.05.2005 - implementacia my_parser.cpp

/*Pali Jancura - to jest ja :)
 * 26.05.2005
 *	- ping prerobeny do dll
 *	- dalsie zmeny, pridany ping, este predtym bol pridany broadcast
 * 20.05.2005
 *	add:	bool my_parser()
 */

#include "stdafx.h"

#include "my_parser.h"
#include "server_thread.h"
#include "broadcast_thread.h"

#include <string>
#include <cctype>
#include <winsock.h>
#include <iostream>
#include <fstream>

#define SPRAVA_SIZE 20

using namespace std;

typedef string::size_type str_size;

typedef size_t (*broadadd_PTR)(char*, size_t, size_t);
typedef size_t (*broadcast_PTR)(char*, size_t);
typedef int (*GetTimeout_PTR)();
typedef bool (*ping_PTR)(struct in_addr, struct in_addr, DWORD, char *);
typedef bool (*newping_PTR)();

broadadd_PTR broadcastadd = NULL;
broadcast_PTR GetBroadcastMsg = NULL;
GetTimeout_PTR GetTimeOutBroadcast= NULL;
ping_PTR ping = NULL;
newping_PTR new_ping = NULL;

//cesta k suboru pre logovanie pingu
char PingLog[MAX_PATH];

/******************************************************************************
 * string word(const string& s, str_size& i)
 *	- zo stringu s, vrati jedno slovo od pozicie i, a pozmeni i, tak ze ukazuje
 *	na zaciatok noveho slova alebo koniec stringu s, za predpokladu, ze kazde 
 *	slovo je oddelene prave jednym bielym znakom
 *	- v inom pripade vrati prazdny retazec
 *	- v configu pouzivam '=',tento znak zatial preskakujem,mozne vyuzitie neskor
 ******************************************************************************/
string word(const string& s, str_size& i)
{
	string vysledok = "";
	str_size j;
	if(i < s.size()){
		j = i;
		//dokial nenarazim na biely znak alebo koniec stringu
		while(j != s.size() && !isspace(s[j])) ++j;
		//bol tam aspon jeden nebiely znak
		if(i != j){
			vysledok = s.substr(i, j - i);
			i = ++j;	//posuniem sa dalej za biely znak
		}
		//preskocim znak '='
        if(vysledok == "=") vysledok = word(s, i);
	}
	return vysledok;
}//string word(string& s, str_size& i)


/******************************************************************************
 * bool my_parser(const string& conf)
 *	- rozobera konfiguracny string pomocou funkcie word a nastavi konfiguraciu
 ******************************************************************************/
bool my_parser(const string& conf)
{
	CLog::WriteLog("parser" , "my_parser(): begin");

	wchar_t pwcLibrary[MAX_PATH];

	char * sprava = new char[SPRAVA_SIZE]; int iNewSize=0;
	string spr = strcpy(sprava, "modular_server 1.0");

	str_size i = 0;
	string option;
	int port1, port2;
	DWORD timeOut = 0; DWORD timeOutBroad = 0;		//default hodnoty
	struct in_addr localhost, subnetmask, broadcastIP;
	bool old = true, pingOLD = false, pingNEW = false; //pre broadcast a ping
	bool bBroadcast=false;

	while(i < conf.size()){
		option = word(conf, i);
		if(option == "port"){
			option = word(conf, i);
			if(option == "telnet"){
				option = word(conf, i);
				port1 = htons(atoi(option.c_str()));
			}else{
			if(option == "broadcast"){
				option = word(conf, i);
				port2 = htons(atoi(option.c_str()));
			}else{
				CLog::WriteLog("parser", "my_parser(): Chyba v nastaveni portov!");
				return false;
			}
			}

		}else{

		if(option == "localhost"){
			option = word(conf, i);
			localhost.s_addr = inet_addr(option.c_str());

		}else{

		if(option == "subnetmask"){
			option = word(conf, i);
			subnetmask.s_addr = inet_addr(option.c_str());
		
		}else{

		if(option == "broadcast"){
      option = word(conf, i);
			if(option == "Default"){
				bBroadcast = true;
				option = word(conf, i);
				timeOutBroad = atoi(option.c_str());
				if(timeOutBroad<=0) {
					CLog::WriteLog("parser", "my_parser(): Chyba v nastaveni: Nenastaveny timeout broadcastu!");
					return false;
				}
			}else{
			if(option == "NEW"){
				bBroadcast = true;
				swprintf(pwcLibrary, L"%sLibrary\\broadcast.dll", pwcProgramFolder);
				HINSTANCE hDll = LoadLibrary(pwcLibrary);
				if(hDll == NULL){
					CLog::WriteLog("parser", "my_parser(): broadcast.dll nie je v library");
					bBroadcast = false;
				}else{
					GetBroadcastMsg = (broadcast_PTR)GetProcAddress(hDll,"GetBroadcastMessage");
					GetTimeOutBroadcast = (GetTimeout_PTR)GetProcAddress(hDll,"GetTimeOut");
					if(GetBroadcastMsg == NULL || GetTimeOutBroadcast==NULL){
						CLog::WriteLog("parser", "my_parser(): Nenacitana funkcia z broadcast.dll");
						bBroadcast = false;
					}else{
						CLog::WriteLog("parser", "my_parser(): GetBroadcastMessage a GetTimeOutBroadcast uspesne importovane!");

						iNewSize = GetBroadcastMsg(sprava, SPRAVA_SIZE);
						if(iNewSize){
							delete [] sprava; sprava = new char [iNewSize+1];
							GetBroadcastMsg(sprava, iNewSize);
							spr = sprava;
						}else spr = sprava;

						timeOutBroad = GetTimeOutBroadcast();
						CLog::WriteLog("parser", "my_parser(): GetBroadcastMessage a GetTimeOutBroadcast uspesne prebehli!");
				 }
				}//if(hDll == NULL)
			}else{
				broadcastIP.s_addr = inet_addr(option.c_str());
			}}

		}else{

		if(option == "broadcastadd"){
			if(!bBroadcast)
				CLog::WriteLog("parser", "my_parser(): Broadcastadd neprebehne: Zle nadefinovany broadcast v dll!");
			else {
				swprintf(pwcLibrary, L"%sLibrary\\broadcastadd.dll", pwcProgramFolder);
				HINSTANCE hDll = LoadLibrary(pwcLibrary);
				if(hDll == NULL){
					CLog::WriteLog("parser", "my_parser(): broadcastadd.dll nie v library!");
				}else{
					broadcastadd=(broadadd_PTR)GetProcAddress(hDll,"broadcastadd");
					if(broadcastadd == NULL){
						CLog::WriteLog("parser", "my_parser(): Nenacitana funkcia z broadcastadd.dll!");
					}else{
						CLog::WriteLog("parser", "my_parser(): Broadcastadd uspesne importovany!");
						if(iNewSize)
							iNewSize = broadcastadd(sprava, iNewSize, spr.size());
						else
							iNewSize = broadcastadd(sprava, SPRAVA_SIZE, spr.size());
						if(iNewSize){
							char * buf = new char[iNewSize+1];
							strcpy(buf, sprava);
							delete [] sprava; sprava = NULL;
							broadcastadd(buf, iNewSize, spr.size());
							spr = buf;
							delete [] buf;
						}else spr = sprava;
					}
				}//if(hDll == NULL)
			}

		}else{

		if(option == "ping"){
			option = word(conf, i);
			if(option == "Default"){
				option = word(conf, i);
				timeOut = atoi(option.c_str());
				if(timeOut<=0) {
					CLog::WriteLog("parser", "my_parser(): Chyba v nastaveni: Nenastaveny timeout pingu!");
					return false;
				}
				pingOLD = true;
			}else
			if(option == "NEW")
				pingNEW = true;

		}else{
			CLog::WriteLog("parser", "my_parser(): Chyba v nastaveni");
			return false;
		}}}}}}
	}//while
	
	//broadcast
	CLog::WriteLog("parser", "my_parser(): Inicializacia broadcastu!");
	sendBroadcastThread * sendBroad = NULL;
	recvBroadcastThread * recvBroad = NULL;
	if(bBroadcast){
		recvBroad = new recvBroadcastThread(port2,localhost);
		sendBroad = new sendBroadcastThread(broadcastIP,port2,spr,timeOutBroad);
	}else{
	}
	if(sprava) delete [] sprava;

	//opingujem siet
	if(pingOLD){
		swprintf(pwcLibrary, L"%sLibrary\\ping.dll", pwcProgramFolder);
		HINSTANCE hDll = LoadLibrary(pwcLibrary);
		if(hDll == NULL){
			CLog::WriteLog("parser", "my_parser(): ping.dll nie v library");
		}else{
			ping = (ping_PTR)GetProcAddress(hDll,"ping");
			if(ping == NULL){
				CLog::WriteLog("parser", "my_parser(): Nenacitana funkcia z ping.dll");
			}else{
				CLog::WriteLog("parser", "my_parser(): ping uspesne importovany!");
				sprintf(PingLog, "%sLog\\ping.log", pcProgramFolder);
				if ( ping(localhost, subnetmask, timeOut, PingLog) ) 
					CLog::WriteLog("parser", "my_parser(): Ping celej siete uspesne prebehol");
				else
					CLog::WriteLog("parser", "my_parser(): Chyba v importovanom ping!");
			}
		}//if(hDll == NULL)
	}
	if(pingNEW){
		swprintf(pwcLibrary, L"%sLibrary\\new_ping.dll", pwcProgramFolder);
		HINSTANCE hDll = LoadLibrary(pwcLibrary);
		if(hDll == NULL){
			CLog::WriteLog("parser", "my_parser(): new_ping.dll nie v library");
		}else{
			new_ping = (newping_PTR)GetProcAddress(hDll,"ping");
			if(new_ping == NULL){
				CLog::WriteLog("parser", "my_parser(): Nenacitana funkcia z new_ping.dll");
			}else{
				CLog::WriteLog("parser", "my_parser(): new_ping uspesne importovany!");
				if(new_ping()) 
					CLog::WriteLog("parser", "my_parser(): Imporotvany new_ping uspesne prebehol");
				else
					CLog::WriteLog("parser", "my_parser(): Chyba v importovanom new_ping!");
			}
		}//if(hDll == NULL)
	}
	//instacionujem triedu pre klientov

	//spustim vlakno pre prijem spojeni na zadanom porte (samotny server)
	CLog::WriteLog("parser", "my_parser(): Allocing listenThread!");
  listenThread * listen = new listenThread(port1, localhost, &Clients);
	CLog::WriteLog("parser", "my_parser(): Alloced listenThread!");
	
	//instacionujem triedu pre spracovanie fronty
	CLog::WriteLog("parser", "my_parser(): Allocing ProcessQueqe!");
	CProcessQueqe* ProcessQueqe = new CProcessQueqe(recvBroad, sendBroad, listen, &Clients);
	CLog::WriteLog("parser", "my_parser(): Alloced ProcessQueqe!");

	//vytvorim zamky pre vlakna pre pristup ku globalnym premennym 
	try{
		CLog::WriteLog("parser", "my_parser(): ProcessQueqe & listen starting!");
		ProcessQueqe->start();
		CLog::WriteLog("parser", "my_parser(): ProcessQueqe started!");
		listen->start();
		CLog::WriteLog("parser", "my_parser(): listen started!");
		//zacnem vysielat a prijmat broadcast
		if(recvBroad) recvBroad->start();
		if(sendBroad) sendBroad->start();
		
		CLog::WriteLog("parser", "my_parser(): listen stoping!");
		listen->stop();
		CLog::WriteLog("parser", "my_parser(): listen stoped!");

		if(recvBroad){
			recvBroad->stop();
			delete recvBroad;
			CLog::WriteLog("parser", "parser: Uspesne zastaveny recvBroad");
		}
		if(sendBroad){
			sendBroad->stop();
			delete sendBroad;
      CLog::WriteLog("parser","parser: Uspesne zastaveny sendBroad");
		}
		ProcessQueqe->stop();
	}catch (ThreadException ex) {
		CLog::WriteLog("parser", (ex.getMessage()).c_str());
	}

	delete listen;
	delete ProcessQueqe;

	return true;
}