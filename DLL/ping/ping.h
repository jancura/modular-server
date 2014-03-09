//26.05.2005 - hlavickovy subor pre ping.cpp

#ifndef MY_PING_H
#define MY_PING_H

#include "stdafx.h"
#include <winsock.h>
#include <fstream>

bool my_ping(struct in_addr, struct in_addr, DWORD, std::ofstream&);

#endif