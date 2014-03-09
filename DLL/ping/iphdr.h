//07.05.2005 - podpora iphdr (pre ip hlavicku), zo serialu Sockety a C++

#ifndef MINGWSUPPORT_IPHDR_DOSTAL
#define MINGWSUPPORT_IPHDR_DOSTAL

typedef struct iphdr
{
    unsigned int ihl : 4;
    unsigned int version : 4;
    unsigned char tos;
    unsigned short int tot_len;
    unsigned short int id;
    unsigned short int frag_off;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short int check;
    unsigned long int saddr;
    unsigned long int daddr;
}iphdr;

#endif
