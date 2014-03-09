//header pre struct sMessage

#ifndef MY_MESSAGE_H
#define MY_MESSAGE_H

#include "stdafx.h"
#include "Codes.h"
#include <string>

enum TYPE {T_PCHAR, T_INT,};

struct sID {
	TYPE type;
	union 
	{
		char  t_name[20];
		int	  t_id;
	};   
};


struct sMessage {
	sID		 nameSender;
	sID		 nameReceiver;
	std::string  Data;
	DWORD	 MessageCode; 
	bool	 Out;
	bool	 Command;
};

#endif