// stdafx.cpp : source file that includes just the standard includes
// ModularServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <time.h>

std::string GetTime()
{
	struct tm *newtime;	__time64_t long_time;	char hour[3], min[3], sec[3];
	_time64( &long_time );                /* Get time as long integer. */
	newtime = _localtime64( &long_time ); /* Convert to local time. */
	//vytvorim string, do ktoreho si zapisem cas hod.min.sec
	return  (std::string)_itoa(newtime->tm_hour, hour, 10) + ":" +
			(std::string)_itoa(newtime->tm_min, min, 10) + ":" +
			(std::string)_itoa(newtime->tm_sec, sec, 10);
}

wchar_t * ConvertCharToWCHAR_T(const char * pChar)
{
	if (pChar)
	{
		int len = strlen(pChar)+1;
		wchar_t *wcChar = new wchar_t[len];

		if (wcChar == 0)	return NULL;

		memset(wcChar,0,len);
		::MultiByteToWideChar(CP_ACP, NULL,pChar, -1, wcChar,len);
		return wcChar;
	}
	return NULL;
}

char * ConvertWChar_tToCHAR(const wchar_t * pWideChar)
{
	if (pWideChar)
	{
		int len = wcslen(pWideChar)+1;
		char* pChar = new char[len];

		if (pChar == 0)	return NULL;

		memset(pChar,0,len);
		::WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, pChar, len, NULL, NULL);
		return pChar;
	}
	return NULL;
}