//04.01.2006 - implementacia program_CLog::cpp, sluzi na rozoberanie vstupu a bufferu

/* Pali Jancura - to jest ja :)
 * 04.01.2006
 *	add: void SaveToFile()
 */

#include "stdafx.h"
#include "program_log.h"

#include <stdio.h>
#include <fstream>

using namespace std;

ofstream LogFile;

void CLog::WriteLog(void* Who, const char* pData)
{
	char Log[BUFSIZE]; strcpy(Log, pData);
	CLog::GetInstance()->Lock(Who);
	CLog::GetInstance()->Write(Who, Log);
	CLog::GetInstance()->Unlock(Who);
}//void CLog::WriteLog(void* pLocker, char* pData)


void CLog::Write(void* pLocker, const char* pData)
{
  if(m_bLocked){ 
    if(pLocker == m_pLocker){
      int iLen = sprintf(m_aLog, pData); 
      Loging(m_aLog, iLen);
    }
  }
}//void CLog::Write(void* pLocker, char* pData)

void CLog::Loging(const char *pData, int iLen)
{
  static CRITICAL_SECTION CritSecFile;
  static bool blFirst = true;

  if(blFirst){
 		InitializeCriticalSection(&CritSecFile);
		sprintf(pLogFile , "%sLog\\program.log", pcProgramFolder);//= GetLogFile();
		blFirst=false;
  }

  EnterCriticalSection(&CritSecFile);
	  
  FILE *fF;
  if(!pData){ 
  /*	fF = fopen(pName, "wt");
  		fclose(fF);
  */	
    LeaveCriticalSection(&CritSecFile);
 	return;
  }

  static bool blAdd = false;
  if(blAdd){
 		fopen_s(&fF, pLogFile, "at");
  }else{
		fopen_s(&fF, pLogFile, "wt");
		blAdd = true;
  }
	
  for(int i = 0; i < iLen; ++i){
		fprintf_s(fF, "%c", pData[i]);
  }//for()
  fprintf_s(fF, "\n");

  fclose(fF);

  LeaveCriticalSection(&CritSecFile);

}//void CLog::Loging(const char *pData, int iLen)

