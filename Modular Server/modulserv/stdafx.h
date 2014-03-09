// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

//prilinkovanie potrebnej kniznice pre podporu winsock
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <string>

#define BUFSIZE 1000		//konstanta velkosti bufferov
#define TIMER	20

typedef int index;

extern wchar_t * pwcProgramFolder;
extern char *		 pcProgramFolder;

#include "program_log.h"
#include "Queqe.h"
#include "Codes.h"

std::string GetTime();
wchar_t * ConvertCharToWCHAR_T(const char * pChar);
char * ConvertWChar_tToCHAR(const wchar_t * pWideChar);
