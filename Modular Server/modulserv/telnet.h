// 12.11.2005 - hlavickovy subor pre implementaciu prikazov protokolu TELNET

/*Pali Jancura - to jest ja :)
 * 15.12.2005
 *	chng: class Telnet {} - add: bool echo, linemode;
 * 1x.12.2005
 *	add: class Telnet {}
 * 12.11.2005
 *	add: zakladne prikazy TELENTu definovane direktivou #define
 */

#ifndef MY_TELNET_H
#define MY_TELNET_H

#include <winsock.h>
//#include "server_thread.h"

//#define BUFSIZE 1000

/****************************************************************************/
////////////////////////////USASCII control codes/////////////////////////////
/****************************************************************************/

#define NUL 0
#define CR 13
#define LF 10

/****************************************************************************/
/////////////////////////prikazy protokolu TELNET/////////////////////////////
/****************************************************************************/
#ifndef IAC 
	#define IAC	255		//uvodny znak prikazov
#endif
#ifndef WILL			//-> "Odosielatel si praje pouzivat nejaku sluzbu"
	#define WILL 251	//<- "Suhlas prijemcu"
#endif
#ifndef WONT			//-> "Odosielatel si praje nepouzivat nejaku sluzbu"
	#define WONT 252	//<- "Nesuhlas prijemcu"
#endif
#ifndef DO				//-> "Odosielatel si praje, aby prijemca pouzil nejaku volbu"
	#define DO 253		//<- "Prijemca potvrdzuje pouzitie volby"
#endif
#ifndef DONT			//-> "Odosielatel si praje, aby prijemca nepouzival nejaku volbu"
	#define DONT 254	//<- "Prijemca odmieta pouzitie volby odosielatelom"
#endif

#ifndef SUSP
	#define SUSP 237	//suspenduj proces - vysli procesu signal SUSP
#endif
#ifndef ABORT
	#define ABORT 238	//ukonci proces signalom ABORT
#endif
#ifndef EOR
	#define EOR 239		//koniec vety (End of record)
#endif
#ifndef SE
	#define SE 240		//koniec parametrov prikazu (Suboption end)
#endif
#ifndef NOP
	#define NOP 241		//prazdna operace (No operation)
#endif
#ifndef DM
	#define DM 242		//znacka pro oznacenie urgetnych dat v TCP segmentu
#endif
#ifndef BRK
	#define BRK 243		//ukonci proces signalom BREAK
#endif
#ifndef IR
	#define IR 244		//ukonci proces signalom  INTERRUPT
#endif
#ifndef AO
	#define AO 245		//zahod vystup
#endif
#ifndef AYT
	#define AYT 246		//si tam? (Are you there?)
#endif
#ifndef EC
	#define EC 247		//unik do prikazoveho riadku
#endif
#ifndef EL
	#define EL 248		//zrus riadok
#endif
#ifndef GA
	#define GA 249		//"Prijem" (Go ahead)
#endif
#ifndef SB
	#define SB 250		//pociatok parametrov prikazu (Suboption begin)
#endif

/*****************************************************************************/
////////////////////////////////protokol TELNET////////////////////////////////
/*****************************************************************************/

#define POSLI 01
#define POSIELAM 00

/*****************************************************************************/
//////////////////////////volby protokolu TELNET///////////////////////////////
/*****************************************************************************/
#ifndef ECHO	
	#define	ECHO 1				//RFC 857
#endif
#ifndef SUPPRES_GO_AHEAD	
	#define	SUPPRES_GO_AHEAD 3	//RFC 858
#endif
#ifndef STATUS	
	#define	STATUS 5			//RFC 859
#endif
#ifndef TIMING_MARK	
	#define	TIMING_MARK 6		//RFC 860
#endif
#ifndef TERMINAL_TYPE	//volba ma dva parametry "01" - posli a "00" - posielam
	#define	TERMINAL_TYPE 24	//RFC 1091
#endif
#ifndef NAWS
	#define	NAWS 31				//RFC 1073
#endif
#ifndef TSPEED
	#define	TSPEED 32			//RFC 1079
#endif
#ifndef LFLOW
	#define	LFLOW 33			//RFC 1372
#endif
#ifndef LINEMODE
	#define	LINEMODE 34			//RFC 1184
#endif
#ifndef XDISPLOC
	#define	XDISPLOC 35			//RFC 1096
#endif
#ifndef OLD_ENVIRON
	#define	OLD_ENVIRON 36		//RFC 1408
#endif
#ifndef NEW_ENVIRON
	#define	NEW_ENVIRON 39		//RFC 1572
#endif
#ifndef STARTTLS //telnet Start TLS
	#define	STARTTLS 46
#endif
#ifndef KERMIT	//telnet Kermit
	#define	KERMIT 47
#endif
#ifndef EXTOP	//rozsireny zoznam volieb
	#define	EXTOP 255			//RFC 861
#endif

/*****************************************************************************/
////////////////////////////////TRIEDA TELNET//////////////////////////////////
/*****************************************************************************/
class Telnet {
private:
	char cLog[BUFSIZE];		//buffer pre log
public:
	Telnet::Telnet();
	//premenna ci sa volba uz pouzila pri vyjednavani
	bool OPT_0_BIN;
	bool OPT_1_ECHO;
	bool OPT_2_RECN;
	bool OPT_3_SUPP;
	bool OPT_4_APRX;
	bool OPT_5_STAT;
	bool OPT_6_TIM;
	bool OPT_7_REM;
	bool OPT_8_OLW;
	bool OPT_9_OPS;
	bool OPT_10_OCRD;
	bool OPT_11_OHT;
	bool OPT_12_OHTD;
	bool OPT_13_OFD;
	bool OPT_14_OVT;
	bool OPT_15_OVTD;
	bool OPT_16_OLD;
	bool OPT_17_EXT;
	bool OPT_18_LOGO;
	bool OPT_19_BYTE;
	bool OPT_20_DATA;
	bool OPT_21_SUP;
	bool OPT_22_SUPO;
	bool OPT_23_SNDL;
	bool OPT_24_TERM;
	bool OPT_25_EOR;
	bool OPT_26_TACACS;
	bool OPT_27_OM;
	bool OPT_28_TLN;
	bool OPT_29_3270;
	bool OPT_30_X3;
	bool OPT_31_NAWS;
	bool OPT_32_TS;
	bool OPT_33_RFC;
	bool OPT_34_LINE;
	bool OPT_35_XDL;
	bool OPT_36_ENVIR;
	bool OPT_37_AUTH;
	//bool OPT_38_;
	bool OPT_39_ENVIR;
	bool OPT_40_TN3270E;
	bool OPT_41_AUTH;
	bool OPT_42_CHARSET;
	bool OPT_43_RSP;
	bool OPT_44_COMPORT;
	bool OPT_45_SLE;
	bool OPT_46_STARTTLS;
	bool OPT_47_KERMIT;
	bool OPT_48_SEND_URL;
	bool OPT_255_EXTOP;

	//pomocne premenne
	bool negotiation;
	bool terminal_type, echo, linemode;
	char terminal[BUFSIZE]; //pre ulozenie typu terminalu klienta
	
	//deklaracia funkcii
	bool telnetCommand(SOCKET, char[], int);
};//class Telnet

#endif //MY_TELNET_H

