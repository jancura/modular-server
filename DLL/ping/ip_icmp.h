//07.05.2005 - podpora icmphdr (pre icmp hlavicku), zo serialu Sockety a C++

#ifndef MINGWSUPPORT_IP_ICMP_DOSTAL
#define MINGWSUPPORT_IP_ICMP_DOSTAL

/* Typy ICMP paketu: */
#ifndef ICMP_ECHOREPLY
    #define ICMP_ECHOREPLY 0
#endif
#ifndef ICMP_ECHO
    #define ICMP_ECHO 8
#endif
#ifndef ICMP_TIME_EXCEEDED
    #define ICMP_TIME_EXCEEDED 11
#endif
#ifndef ICMP_DEST_UNREACH
    #define ICMP_DEST_UNREACH 3
#endif
/* Dalsi je mozne dopsat. */

/* Podtypy ICMP paketu: */
#ifndef ICMP_EXC_TTL
    #define ICMP_EXC_TTL 0
#endif
#ifndef ICMP_PORT_UNREACH
    #define ICMP_PORT_UNREACH 3
#endif
/* Dalsi je mozne dopsat. */

struct icmphdr
{
  unsigned char type;
  unsigned char code;
  unsigned short int checksum;
  union
  {
    struct
    {
      DWORD id;
      unsigned short int sequence;
    } echo;
    /* Dalsi vetve unie lze dopsat. */
 } un;
};

#endif
