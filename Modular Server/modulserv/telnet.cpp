// 13.11.2005 - implementacia protokolu TELNET

/*Pali Jancura - to jest ja :)
 * 15.12.2005
 *	add&chng: Telnet::Telnet()
 *	chng: initialization -> negotiation
 *		  telnetCommand() - pridane spracovanie echa a linemode
 * 13.11.2005
 *	- implemetacia protokolu TELNET
 *	add: telnetCommand()
 */

#include "stdafx.h"

#include "telnet.h"

#include <winsock.h>
#include <iostream>
#include <string>

// char terminal[BUFSIZE];
Telnet::Telnet()
{
	negotiation = false;
	terminal_type = false;
	echo = false;
	linemode = false;
	
	OPT_0_BIN = false;
	OPT_1_ECHO = false;
	OPT_2_RECN = false;
	OPT_3_SUPP = false;
	OPT_4_APRX = false;
	OPT_5_STAT = false;
	OPT_6_TIM = false;
	OPT_7_REM = false;
	OPT_8_OLW = false;
	OPT_9_OPS = false;
	OPT_10_OCRD = false;
	OPT_11_OHT = false;
	OPT_12_OHTD = false;
	OPT_13_OFD = false;
	OPT_14_OVT = false;
	OPT_15_OVTD = false;
	OPT_16_OLD = false;
	OPT_17_EXT = false;
	OPT_18_LOGO = false;
	OPT_19_BYTE = false;
	OPT_20_DATA = false;
	OPT_21_SUP = false;
	OPT_22_SUPO = false;
	OPT_23_SNDL = false;
	OPT_24_TERM = false;
	OPT_25_EOR = false;
	OPT_26_TACACS = false;
	OPT_27_OM = false;
	OPT_28_TLN = false;
	OPT_29_3270 = false;
	OPT_30_X3 = false;
	OPT_31_NAWS = false;
	OPT_32_TS = false;
	OPT_33_RFC = false;
	OPT_34_LINE = false;
	OPT_35_XDL = false;
	OPT_36_ENVIR = false;
	OPT_37_AUTH = false;
	//OPT_38_;
	OPT_39_ENVIR = false;
	OPT_40_TN3270E = false;
	OPT_41_AUTH = false;
	OPT_42_CHARSET = false;
	OPT_43_RSP = false;
	OPT_44_COMPORT = false;
	OPT_45_SLE = false;
	OPT_46_STARTTLS = false;
	OPT_47_KERMIT = false;
	OPT_48_SEND_URL = false;
	OPT_255_EXTOP = false;
}
//*/
using namespace std;
//using namespace check_telnet_option;


bool Telnet::telnetCommand(SOCKET socket, char message[], int size)
{
	char answer[BUFSIZE];	
	BYTE* p = (BYTE*)answer;
	int velkost = 0;

	for (int i = 0; i < size; ++i) {
		switch ((BYTE)message[i])
		{
			case IAC :
				break;
			case WILL :
				++i;
				if(i < size){
					switch (BYTE(message[i]))
					{
						case 0 :
							if(!OPT_0_BIN){
								OPT_0_BIN = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case ECHO :
							if(!OPT_1_ECHO){
								OPT_1_ECHO= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}else{
								if(negotiation)//vypnem echo, bude ho robit klient sam
									echo = false; 
							}
							break;
						case 2 :
							if(!OPT_2_RECN){
								OPT_2_RECN = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case SUPPRES_GO_AHEAD :
							if(!OPT_3_SUPP){
								OPT_3_SUPP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 4 :
							if(!OPT_4_APRX){
								OPT_4_APRX = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STATUS :
							if(!OPT_5_STAT){
								OPT_5_STAT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TIMING_MARK :
							if(!OPT_6_TIM){
								OPT_6_TIM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 7 :
							if(!OPT_7_REM){
								OPT_7_REM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 8 :
							if(!OPT_8_OLW){
								OPT_8_OLW = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 9 :
							if(!OPT_9_OPS){
								OPT_9_OPS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 10 :
							if(!OPT_10_OCRD){
								OPT_10_OCRD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 11 :
							if(!OPT_11_OHT){
								OPT_11_OHT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 12 :
							if(!OPT_12_OHTD){
								OPT_12_OHTD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 13 :
							if(!OPT_13_OFD){
								OPT_13_OFD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 14 :
							if(!OPT_14_OVT){
								OPT_14_OVT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 15 :
							if(!OPT_15_OVTD){
								OPT_15_OVTD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 16 :
							if(!OPT_16_OLD){
								OPT_16_OLD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 17 :
							if(!OPT_17_EXT){
								OPT_17_EXT= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 18 :
							if(!OPT_18_LOGO){
								OPT_18_LOGO = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 19 :
							if(!OPT_19_BYTE){
								OPT_19_BYTE= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 20 :
							if(!OPT_20_DATA){
								OPT_20_DATA = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 21 :
							if(!OPT_21_SUP){
								OPT_21_SUP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 22 :
							if(!OPT_22_SUPO){
								OPT_22_SUPO = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 23 :
							if(!OPT_23_SNDL){
								OPT_23_SNDL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TERMINAL_TYPE :
							if(!OPT_24_TERM){
								OPT_24_TERM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}else{ //prijem odpovedi na moj poziadavok o typ teminalu
								if(negotiation){//zasli mi typ svojho terminalu
									*p++ = IAC; *p++ = SB;
									*p++ = TERMINAL_TYPE; *p++ = POSLI;
									*p++ = IAC; *p++ = SE;
									velkost += 6;
								}//if(terminal)
							}//if(!OPT_24_TERM)
							break;
						case 25 :
							if(!OPT_25_EOR){
								OPT_25_EOR = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 26 :
							if(!OPT_26_TACACS){
								OPT_26_TACACS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 27 :
							if(!OPT_27_OM){
								OPT_27_OM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 28 :
							if(!OPT_28_TLN){
								OPT_28_TLN= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 29 :
							if(!OPT_29_3270){
								OPT_29_3270 = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 30 :
							if(!OPT_30_X3){
								OPT_30_X3 = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NAWS :
							if(!OPT_31_NAWS){
								OPT_31_NAWS= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TSPEED :
							if(!OPT_32_TS){
								OPT_32_TS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LFLOW :
							if(!OPT_33_RFC){
								OPT_33_RFC = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LINEMODE :
							if(!OPT_34_LINE){
								OPT_34_LINE = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case XDISPLOC :
							if(!OPT_35_XDL){
								OPT_35_XDL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case OLD_ENVIRON :
							if(!OPT_36_ENVIR){
								OPT_36_ENVIR = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 37 :
							if(!OPT_37_AUTH){
								OPT_37_AUTH = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NEW_ENVIRON :
							if(!OPT_39_ENVIR){
								OPT_39_ENVIR= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 40 :
							if(!OPT_40_TN3270E){
								OPT_40_TN3270E = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 41 :
							if(!OPT_41_AUTH){
								OPT_41_AUTH = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 42 :
							if(!OPT_42_CHARSET){
								OPT_42_CHARSET = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 43 :
							if(!OPT_43_RSP){
								OPT_43_RSP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 44 :
							if(!OPT_44_COMPORT){
								OPT_44_COMPORT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 45 :
							if(!OPT_45_SLE){
								OPT_45_SLE = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STARTTLS :
							if(!OPT_46_STARTTLS){
								OPT_46_STARTTLS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case KERMIT :
							if(!OPT_47_KERMIT){
								OPT_47_KERMIT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 48 :
							if(!OPT_48_SEND_URL){
								OPT_48_SEND_URL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case EXTOP :
							if(!OPT_255_EXTOP){
								OPT_255_EXTOP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						default:
							CLog::WriteLog("telnet", "telnetCommand(): Unknow telnet option!");
					}//switch{}
				}//if()
				break;//WILLL */
			case WONT :
				++i;
				if(i < size){
					switch (BYTE(message[i]))
					{
						case 0 :
							if(!OPT_0_BIN){
								OPT_0_BIN = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case ECHO :
							if(!OPT_1_ECHO){
								OPT_1_ECHO = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}else{//ak uz bola volba pouzita pri vyjednavani z 
								if(negotiation)                 //mojej strany
									echo = true; //musim robit echo :(
							}
							break;
						case 2 :
							if(!OPT_2_RECN){
								OPT_2_RECN = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case SUPPRES_GO_AHEAD :
							if(!OPT_3_SUPP){
								OPT_3_SUPP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 4 :
							if(!OPT_4_APRX){
								OPT_4_APRX = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STATUS :
							if(!OPT_5_STAT){
								OPT_5_STAT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TIMING_MARK :
							if(!OPT_6_TIM){
								OPT_6_TIM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 7 :
							if(!OPT_7_REM){
								OPT_7_REM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 8 :
							if(!OPT_8_OLW){
								OPT_8_OLW = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 9 :
							if(!OPT_9_OPS){
								OPT_9_OPS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 10 :
							if(!OPT_10_OCRD){
								OPT_10_OCRD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 11 :
							if(!OPT_11_OHT){
								OPT_11_OHT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 12 :
							if(!OPT_12_OHTD){
								OPT_12_OHTD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 13 :
							if(!OPT_13_OFD){
								OPT_13_OFD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 14 :
							if(!OPT_14_OVT){
								OPT_14_OVT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 15 :
							if(!OPT_15_OVTD){
								OPT_15_OVTD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 16 :
							if(!OPT_16_OLD){
								OPT_16_OLD = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 17 :
							if(!OPT_17_EXT){
								OPT_17_EXT= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 18 :
							if(!OPT_18_LOGO){
								OPT_18_LOGO = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 19 :
							if(!OPT_19_BYTE){
								OPT_19_BYTE= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 20 :
							if(!OPT_20_DATA){
								OPT_20_DATA = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 21 :
							if(!OPT_21_SUP){
								OPT_21_SUP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 22 :
							if(!OPT_22_SUPO){
								OPT_22_SUPO = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 23 :
							if(!OPT_23_SNDL){
								OPT_23_SNDL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TERMINAL_TYPE :
							if(!OPT_24_TERM){
								OPT_24_TERM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 25 :
							if(!OPT_25_EOR){
								OPT_25_EOR = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 26 :
							if(!OPT_26_TACACS){
								OPT_26_TACACS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 27 :
							if(!OPT_27_OM){
								OPT_27_OM = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 28 :
							if(!OPT_28_TLN){
								OPT_28_TLN= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 29 :
							if(!OPT_29_3270){
								OPT_29_3270 = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 30 :
							if(!OPT_30_X3){
								OPT_30_X3 = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NAWS :
							if(!OPT_31_NAWS){
								OPT_31_NAWS= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TSPEED :
							if(!OPT_32_TS){
								OPT_32_TS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LFLOW :
							if(!OPT_33_RFC){
								OPT_33_RFC = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LINEMODE :
							if(!OPT_34_LINE){
								OPT_34_LINE = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case XDISPLOC :
							if(!OPT_35_XDL){
								OPT_35_XDL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case OLD_ENVIRON :
							if(!OPT_36_ENVIR){
								OPT_36_ENVIR = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 37 :
							if(!OPT_37_AUTH){
								OPT_37_AUTH = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NEW_ENVIRON :
							if(!OPT_39_ENVIR){
								OPT_39_ENVIR= true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 40 :
							if(!OPT_40_TN3270E){
								OPT_40_TN3270E = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 41 :
							if(!OPT_41_AUTH){
								OPT_41_AUTH = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 42 :
							if(!OPT_42_CHARSET){
								OPT_42_CHARSET = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 43 :
							if(!OPT_43_RSP){
								OPT_43_RSP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 44 :
							if(!OPT_44_COMPORT){
								OPT_44_COMPORT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 45 :
							if(!OPT_45_SLE){
								OPT_45_SLE = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STARTTLS :
							if(!OPT_46_STARTTLS){
								OPT_46_STARTTLS = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case KERMIT :
							if(!OPT_47_KERMIT){
								OPT_47_KERMIT = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 48 :
							if(!OPT_48_SEND_URL){
								OPT_48_SEND_URL = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case EXTOP :
							if(!OPT_255_EXTOP){
								OPT_255_EXTOP = true;
								*p++ = IAC;
								*p++ = DONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						default:
							CLog::WriteLog("telnet", "telnetCommand(): Unknow telnet option!");
					}//switch{}
				}//if()
				break;//WONT */
			case DO :
				++i;
				if(i < size){
					switch (BYTE(message[i]))
					{
						case 0 :
							if(!OPT_0_BIN){
								OPT_0_BIN = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case ECHO :
							if(!OPT_1_ECHO){
								OPT_1_ECHO= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 2 :
							if(!OPT_2_RECN){
								OPT_2_RECN = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case SUPPRES_GO_AHEAD :
							if(!OPT_3_SUPP){
								OPT_3_SUPP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 4 :
							if(!OPT_4_APRX){
								OPT_4_APRX = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STATUS :
							if(!OPT_5_STAT){
								OPT_5_STAT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TIMING_MARK :
							if(!OPT_6_TIM){
								OPT_6_TIM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 7 :
							if(!OPT_7_REM){
								OPT_7_REM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 8 :
							if(!OPT_8_OLW){
								OPT_8_OLW = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 9 :
							if(!OPT_9_OPS){
								OPT_9_OPS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 10 :
							if(!OPT_10_OCRD){
								OPT_10_OCRD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 11 :
							if(!OPT_11_OHT){
								OPT_11_OHT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 12 :
							if(!OPT_12_OHTD){
								OPT_12_OHTD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 13 :
							if(!OPT_13_OFD){
								OPT_13_OFD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 14 :
							if(!OPT_14_OVT){
								OPT_14_OVT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 15 :
							if(!OPT_15_OVTD){
								OPT_15_OVTD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 16 :
							if(!OPT_16_OLD){
								OPT_16_OLD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 17 :
							if(!OPT_17_EXT){
								OPT_17_EXT= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 18 :
							if(!OPT_18_LOGO){
								OPT_18_LOGO = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 19 :
							if(!OPT_19_BYTE){
								OPT_19_BYTE= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 20 :
							if(!OPT_20_DATA){
								OPT_20_DATA = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 21 :
							if(!OPT_21_SUP){
								OPT_21_SUP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 22 :
							if(!OPT_22_SUPO){
								OPT_22_SUPO = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 23 :
							if(!OPT_23_SNDL){
								OPT_23_SNDL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TERMINAL_TYPE :
							if(!OPT_24_TERM){
								OPT_24_TERM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 25 :
							if(!OPT_25_EOR){
								OPT_25_EOR = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 26 :
							if(!OPT_26_TACACS){
								OPT_26_TACACS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 27 :
							if(!OPT_27_OM){
								OPT_27_OM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 28 :
							if(!OPT_28_TLN){
								OPT_28_TLN= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 29 :
							if(!OPT_29_3270){
								OPT_29_3270 = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 30 :
							if(!OPT_30_X3){
								OPT_30_X3 = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NAWS :
							if(!OPT_31_NAWS){
								OPT_31_NAWS= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TSPEED :
							if(!OPT_32_TS){
								OPT_32_TS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LFLOW :
							if(!OPT_33_RFC){
								OPT_33_RFC = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LINEMODE :
							if(!OPT_34_LINE){
								OPT_34_LINE = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case XDISPLOC :
							if(!OPT_35_XDL){
								OPT_35_XDL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case OLD_ENVIRON :
							if(!OPT_36_ENVIR){
								OPT_36_ENVIR = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 37 :
							if(!OPT_37_AUTH){
								OPT_37_AUTH = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NEW_ENVIRON :
							if(!OPT_39_ENVIR){
								OPT_39_ENVIR= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 40 :
							if(!OPT_40_TN3270E){
								OPT_40_TN3270E = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 41 :
							if(!OPT_41_AUTH){
								OPT_41_AUTH = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 42 :
							if(!OPT_42_CHARSET){
								OPT_42_CHARSET = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 43 :
							if(!OPT_43_RSP){
								OPT_43_RSP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 44 :
							if(!OPT_44_COMPORT){
								OPT_44_COMPORT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 45 :
							if(!OPT_45_SLE){
								OPT_45_SLE = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STARTTLS :
							if(!OPT_46_STARTTLS){
								OPT_46_STARTTLS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case KERMIT :
							if(!OPT_47_KERMIT){
								OPT_47_KERMIT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 48 :
							if(!OPT_48_SEND_URL){
								OPT_48_SEND_URL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case EXTOP :
							if(!OPT_255_EXTOP){
								OPT_255_EXTOP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						default:
							CLog::WriteLog("telnet", "telnetCommand(): Unknow telnet option!");
					}//switch{}
				}//if()
				break;//DO */
			case DONT :
				//message[i] = WONT;
				++i;
				if(i < size){
					switch (BYTE(message[i]))
					{
						case 0 :
							if(!OPT_0_BIN){
								OPT_0_BIN = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case ECHO :
							if(!OPT_1_ECHO){
								OPT_1_ECHO= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 2 :
							if(!OPT_2_RECN){
								OPT_2_RECN = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case SUPPRES_GO_AHEAD :
							if(!OPT_3_SUPP){
								OPT_3_SUPP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ = BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 4 :
							if(!OPT_4_APRX){
								OPT_4_APRX = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STATUS :
							if(!OPT_5_STAT){
								OPT_5_STAT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TIMING_MARK :
							if(!OPT_6_TIM){
								OPT_6_TIM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 7 :
							if(!OPT_7_REM){
								OPT_7_REM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 8 :
							if(!OPT_8_OLW){
								OPT_8_OLW = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 9 :
							if(!OPT_9_OPS){
								OPT_9_OPS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 10 :
							if(!OPT_10_OCRD){
								OPT_10_OCRD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 11 :
							if(!OPT_11_OHT){
								OPT_11_OHT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 12 :
							if(!OPT_12_OHTD){
								OPT_12_OHTD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 13 :
							if(!OPT_13_OFD){
								OPT_13_OFD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 14 :
							if(!OPT_14_OVT){
								OPT_14_OVT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 15 :
							if(!OPT_15_OVTD){
								OPT_15_OVTD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 16 :
							if(!OPT_16_OLD){
								OPT_16_OLD = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 17 :
							if(!OPT_17_EXT){
								OPT_17_EXT= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 18 :
							if(!OPT_18_LOGO){
								OPT_18_LOGO = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 19 :
							if(!OPT_19_BYTE){
								OPT_19_BYTE= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 20 :
							if(!OPT_20_DATA){
								OPT_20_DATA = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 21 :
							if(!OPT_21_SUP){
								OPT_21_SUP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 22 :
							if(!OPT_22_SUPO){
								OPT_22_SUPO = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 23 :
							if(!OPT_23_SNDL){
								OPT_23_SNDL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TERMINAL_TYPE :
							if(!OPT_24_TERM){
								OPT_24_TERM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 25 :
							if(!OPT_25_EOR){
								OPT_25_EOR = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 26 :
							if(!OPT_26_TACACS){
								OPT_26_TACACS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 27 :
							if(!OPT_27_OM){
								OPT_27_OM = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 28 :
							if(!OPT_28_TLN){
								OPT_28_TLN= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 29 :
							if(!OPT_29_3270){
								OPT_29_3270 = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 30 :
							if(!OPT_30_X3){
								OPT_30_X3 = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NAWS :
							if(!OPT_31_NAWS){
								OPT_31_NAWS= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case TSPEED :
							if(!OPT_32_TS){
								OPT_32_TS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LFLOW :
							if(!OPT_33_RFC){
								OPT_33_RFC = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case LINEMODE :
							if(!OPT_34_LINE){
								OPT_34_LINE = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case XDISPLOC :
							if(!OPT_35_XDL){
								OPT_35_XDL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case OLD_ENVIRON :
							if(!OPT_36_ENVIR){
								OPT_36_ENVIR = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 37 :
							if(!OPT_37_AUTH){
								OPT_37_AUTH = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case NEW_ENVIRON :
							if(!OPT_39_ENVIR){
								OPT_39_ENVIR= true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 40 :
							if(!OPT_40_TN3270E){
								OPT_40_TN3270E = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 41 :
							if(!OPT_41_AUTH){
								OPT_41_AUTH = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 42 :
							if(!OPT_42_CHARSET){
								OPT_42_CHARSET = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 43 :
							if(!OPT_43_RSP){
								OPT_43_RSP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 44 :
							if(!OPT_44_COMPORT){
								OPT_44_COMPORT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 45 :
							if(!OPT_45_SLE){
								OPT_45_SLE = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case STARTTLS :
							if(!OPT_46_STARTTLS){
								OPT_46_STARTTLS = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case KERMIT :
							if(!OPT_47_KERMIT){
								OPT_47_KERMIT = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case 48 :
							if(!OPT_48_SEND_URL){
								OPT_48_SEND_URL = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						case EXTOP :
							if(!OPT_255_EXTOP){
								OPT_255_EXTOP = true;
								*p++ = IAC;
								*p++ = WONT;
								*p++ =BYTE(message[i]);
								velkost += 3;
							}
							break;
						default:
							CLog::WriteLog("telnet", "telnetCommand(): Unknow telnet option!");
					}//switch{}
				}//if()
				break;//DONT */
			case SB :
				++i;
				if(i < size){
					switch (BYTE(message[i]))
					{
						case TERMINAL_TYPE :
							++i;
							if(i < size){
								if(BYTE(message[i]) == POSIELAM){
									++i; 
									if(i < size){//??????????
										int j=0;
									  for(j=i;j < size;++j){
 									    if(BYTE(message[j]) == IAC){
                                          break;
										}
									  }//for()
									  copy(message+i,message+j,terminal);
									  *(terminal + (j-i)) = '\0';
									  //vypis terminalu
										char pcTerminal[BUFSIZE];
										sprintf(pcTerminal, "telnetCommand(): Typ terminalu: %s !", terminal);
									  CLog::WriteLog("telnet", pcTerminal);
									}//if()
								}//if()
							}//if()
							break;
						default: ;
					}//switch()
				}//if()
				break;
			case SE:
				break;
			default: ;
		}//switch()
	}//for()*/

	//INICIALIZACIA SPOJENIA CEZ TELNET PRE ZISTENIE TYPU KLIENTA
	if(!negotiation){
		negotiation = true;//vyjednavam
		*p++   = IAC; *p++ = DO;
		*p++ = TERMINAL_TYPE;//*
		OPT_24_TERM = true; //znacim si ze volba uz bola pouzita*/
		velkost += 3;
	}//if
	
	if(velkost && velkost < BUFSIZE){
		if(send(socket, answer, velkost, 0) == SOCKET_ERROR){
			CLog::WriteLog("telnet", "telnetCommand(): Problém s odoslanim dat!");
			return false;
		}
	}

	return true;

}//void telnetmessage()
