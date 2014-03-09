//18.05.2005 - hlavickovy subor pre exe_cmd.cpp

/* Pali Jancura - to jest ja :)
 * 18.05.2005
 *	add:	bool execute();
 *			std::vector<std::string> spracuj();
 *			std::istream& parser_cin();	
 */

#ifndef MY_EXE_CMD
#define MY_EXE_CMD

#include "stdafx.h"

#include "server_thread.h"

#include <iostream>
#include <vector>
#include <string>

//void execute(std::vector<std::string>&, );
std::vector<std::string> spracuj(const std::string&);
//std::istream& parser_cin(std::istream&, stopCommunicateThread*, listenThread*);
//void execute_tel(std::vector<std::string>& command, stopCommunicateThread *,
//		 listenThread *, communicateThread *, std::vector<communicateThread*>&);

#endif