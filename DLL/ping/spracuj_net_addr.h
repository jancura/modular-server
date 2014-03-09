// hlavickovy subor s deklaraciou funkcii pre spracovanie IP adries

#ifndef SPRACUJ_NET_ADDR
#define SPRACUJ_NET_ADDR

#include "stdafx.h"
#include <stdexcept>


int getLocalIP(struct in_addr *in);
struct in_addr networkAddress(struct in_addr addr, struct in_addr subnet);
int spracujSubnet(struct in_addr subnet);
int generujIP(struct in_addr* in);
int inicializujGenerovanie(struct in_addr localIP, struct in_addr subnet);

#endif