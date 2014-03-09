//18.05.2005 - implementacia exe_cmd.cpp, sluzi na rozoberanie vstupu a bufferu

/* Pali Jancura - to jest ja :)
 * 15.12.2005
 *	chng:	bool execute()
 *				- chyba pri porovnani if(amI->telnet.terminal != "XTERM") - ???
 *			vector<string> spracuj(const string& line) 
 *				- pridane rozpoznanie noveho riadka v TELNETu
 * 19.05.2005
 *	chng:	bool execute()
 *				- pridane alebo trosku pozmene prikazy close, who, whoami
 *				- ak ide o prikaz ktory nepozna, rozposle vsetkym spravu, ze od
 *				  koho, a co
 * 18.05.2005
 *	add:	vector<string> spracuj(const string& line)
 *			istream& parser_cin(istream& vstup)
 *			bool execute()
 */

#include "stdafx.h"

#include "exe_cmd.h"
//#include "program_log.h"

#include "server_thread.h"

#include <iostream>
#include <fstream> 
#include <string>
#include <cctype>
#include <vector>

using namespace std;

typedef vector<string>::size_type str_size;


/******************************************************************************
 * void execute(const vector<string>& command)
 *	- prejde command, pozrie ci tam je prikaz, ak ano spusti ho
 *	- vracia true, ak doslo prikazu, ze server ma byt zatvoreny (~close)
 *****************************************************************************/
void communicateThread::Execute(vector<string> command)
{	
	//char arg[10]; //koli funkcii _itoa_s()
	str_size i = 0;
	char msg[BUFSIZE];

	if (command.size()>0) { //su nejake commady abo blbosti
		//prikaz close, zavrie server, predtym posle vsetkym o tom spravu
		if(command[0] == "$close" && command.size()==1) 
		{
			sprintf(msg, "communicateThread::Execute : Command close from: %s, client ID: %i", getIP(), m_sID.t_id);
			CLog::WriteLog(this, msg);
			sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"ALL");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, END);
			return;
		}
		//prikaz who, posle ziadatelovi zoznam vestkych pripojenych klientov
		if(command[0] == "$who" && command.size()==1)
		{
			sprintf(msg, "communicateThread::execute : Command who from: %s, client ID: %i", getIP(), m_sID.t_id);
			CLog::WriteLog(this, msg);
			sID Receiver;  Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"Clients");
			CMessageQueqe::PushBackMessageToQueqe(m_sID, Receiver, WHO);
			return;
		}
		//prikaz whoami, posle ziadatelovi info o nom
		if(command[0] == "$whoami" && command.size()==1)
		{
			int iLen=sprintf(msg, "%s\t, client ID: %i", getIP(), m_sID.t_id);
			Send(msg, iLen);
			return;
		}
		//prikaz na server pre ukoncenie spojenia s klientom
		if (command[0] == "$exit" && command.size()==1) 
		{
			CLog::WriteLog(this, "communicateThread::execute : Prerusenie spojenia s klientom na command exit!");
			int iLen=sprintf(msg, "Connection is closed!"); 
			end=YES; 
			Send(msg, iLen);
			sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"Clients");
			CMessageQueqe::PushBackMessageToQueqe(m_sID,Receiver,END);
			return;
		}

		CommandHandler hndCommand=Commands.GetHandler(command[0].c_str());
		if (hndCommand) 
		{
			size_t iSize=0;
			for (size_t i=0; i<command.size(); ++i) {
				iSize += command[i].size(); ++iSize; //medzera
			}
			char* Command=new char[iSize+1];
			strcpy(Command, command[0].c_str()); 
			for (size_t j=1; j<command.size(); ++j) {
				strcat(Command, " "); strcat(Command, command[j].c_str());
			}
			RunOutCommand(Command, hndCommand);
			delete [] Command;
		}
		else
			Send(message, recvsize);

		return;	
	}//if (command.size()>0)
	//Send(" ",0);
	return;	
}



/******************************************************************************
 * vector<string> spracuj(const string& line) 
 *	- spracuje string a rozdeli ho na slova odelene bielymi znakmi, vrati vector
 *	  tychto slov (slovo = suvisly sled znakov neprerusenych bielym znakom
 *****************************************************************************/
vector<string> spracuj(const string& line)
{
	vector<string> vysledok;
	str_size i = 0, j;

	//dokial nespracujem cely riadok
	while(i != line.size()){
		// prechadzam biele znaky dokial nenarazim na znak
		while(i != line.size() && isspace(line[i])) ++i;
		
		//novy riadok TELNET = CRLF
		if(i>1 && line[i-2]==CR && line[i-1]==LF){
			vysledok.push_back(line.substr(i-2, i));
			CLog::WriteLog("spracuj", "spracuj(): spracovane znaky CR LF");
			return vysledok;
		}

		//najdem koniec slova v stringu od pozicie i 
		j = i;
		while(j != line.size() && !isspace(line[j])) ++j; //chyba telnet

		//zlozime slovo do stringu a ulozime do vysledneho vectoru
		if(i != j){
			vysledok.push_back(line.substr(i, j - i));
			i = j;
		}
	}//while

	return vysledok;
}//spracuj()
