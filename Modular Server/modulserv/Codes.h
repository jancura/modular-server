//header pre codovanie sprav a commandov

#ifndef MY_CODES
#define MY_CODES

//#include "StdAfx.h"

enum CODE { 
  NO		,
  YES		,
  OTHER ,
};

//Message Code
#define	END							0x00
#define	WHO							0x01
#define	YOUARE					0x02
#define	END_SOCKCLOSED	0x03	//ked je socket zavrety



//commady
//#define HOWAMI	0x00000001
//#define HOW		0x00000002
//#define CLOSE	0x00000004

#endif 