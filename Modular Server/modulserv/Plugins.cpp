 #include "stdafx.h"

#include "Plugins.h"
#include "HandlerRegister.h"
 
#include <stdio.h> 
#include <stdlib.h> 
#include <direct.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

#define LONG_LINE 128

HandlerRegister<CommandHandler>  Commands;	
HandlerRegister<FunctionHandler> Functions;
HandlerRegister<ProcessHandler>  Processes;	
HandlerRegister<SendMsgHandler>  SendingFunction;
HandlerRegister<ProcCmdHandler>  ProcessCmdMsg;

std::vector<int>	vecCodes;

CODE finddll(char * pBuff, wchar_t DLL[], int iSize)
{
	char dll[LONG_LINE];
	int size=0; int lenght=strlen(pBuff);
	if (iSize > 5) {
		int j=0;int i=0; 
		while(i<lenght) {
			while(i<lenght && isspace(pBuff[i])) ++i; //preskocim biele znaky
			j=i;
			while(i<lenght && !isspace(pBuff[i])) ++i;
			if(i!=j) { 
				size=i-j; 
				strncpy(dll, pBuff+j, size); 
				dll[size]='\0';
			}
			if(size && size>4 && 
				((dll[size-1]=='l' && dll[size-2]=='l' && dll[size-3]=='d' && dll[size-4]=='.') ||
				(dll[size-1]=='L' && dll[size-2]=='L' && dll[size-3]=='D' && dll[size-4]=='.')))
			{
				CLog::WriteLog("finddll", "finddll(): Zacinam convertovat char* na wchar_t* !");
				wchar_t * pwcDLL = ConvertCharToWCHAR_T(dll);
				CLog::WriteLog("finddll", "finddll(): Zkonvertovane!");
				if (pwcDLL) {
					wcscpy(DLL, pwcDLL);
					CLog::WriteLog("finddll", "finddll(): Skopirovany nazov dll do parametru DLL!");
					delete [] pwcDLL;
					return YES;
				} else 
					return OTHER;
			}
			size=0; strcpy(dll, "");
		}
		return NO;
	}
	return OTHER;
}


void Load(wchar_t * dllpath)
{	
	CLog::WriteLog("Load", "Load(): begin");
	CommandHandler	 fCommandHandler =NULL;   GetNameHandler fGetNameHandler   =NULL;
	FunctionHandler  fFunctionHandler=NULL;   SendMsgHandler fSendingFunction  =NULL;
	ProcessHandler	 fProcessHandler =NULL;   GetCodeHandler fGetCodeHandler   =NULL;
	GetNameHandler   fGetIdHandler	 =NULL;		ProcCmdHandler fProcessCmdHandler=NULL;

	HINSTANCE hDll = LoadLibrary(dllpath);
  if (hDll == NULL) return;
	CLog::WriteLog("Load", "Load(): Nacitana library!");
    
	fCommandHandler = (CommandHandler)GetProcAddress(hDll,"Command");
	if (fCommandHandler) {
		CLog::WriteLog("Load", "Load(): Command!");
		fGetNameHandler = (GetNameHandler)GetProcAddress(hDll,"GetName"); //ak nema command meno ani ho neregistrujem
		if (fGetNameHandler) 
		{
			CLog::WriteLog("Load", "Load(): GetCommand!");
			char* pCommandName = new char [SIZE_NAME];
			CLog::WriteLog("Load", "Load(): Naalokovany pCommandName");

			size_t iSize=0;
			if (pCommandName) iSize=fGetNameHandler(pCommandName, SIZE_NAME);
			else CLog::WriteLog("Load", "Load(): pCommandName NULL!!");

			CLog::WriteLog("Load", "Load(): GetNameHandler OK!");
			if (iSize) {
				CLog::WriteLog("Load", "Load(): GetNameHandler reintializing!");
				delete [] pCommandName; pCommandName=new char[iSize+1];
				CLog::WriteLog("Load", "Load(): GetNameHandler realloc OK!");
				fGetNameHandler(pCommandName, iSize);
				CLog::WriteLog("Load", "Load(): GetNameHandler OK reinitializing!");
			}
			CLog::WriteLog("Load", "Load(): GetnameHandler prebehol v poriadku!");
			Commands.Register(fCommandHandler, pCommandName);
			CLog::WriteLog("Load", "Load(): Command register prebehol v poriadku!");
			delete [] pCommandName; 
			CLog::WriteLog("Load", "Load(): Command name deleted!");
		} 
		return;
	}

	fProcessCmdHandler=(ProcCmdHandler)GetProcAddress(hDll,"ProcessCommandMessage");
	if (fProcessCmdHandler) {
		CLog::WriteLog("Load", "Load(): ProcessCommandMessage");
		fGetCodeHandler = (GetCodeHandler)GetProcAddress(hDll,"GetCode");	//Process moze byt definovany jedine k nejakemu konkretnemu codu
		if (fGetCodeHandler) 
		{
			CLog::WriteLog("Load", "Load(): getCode");
			int Code=fGetCodeHandler(); char arg[10];
			for (size_t i=0; i<vecCodes.size(); ++i) {
				if (vecCodes[i]==Code) return; //pre dany kod uz je definovana funkcia, jeden cod=jedna funkcia ProcessCode
			}
			vecCodes.push_back(Code);
			CLog::WriteLog("Load", "Load(): Zapisal som si Code");
			ProcessCmdMsg.Register(fProcessCmdHandler, _itoa(Code,arg,10));
			CLog::WriteLog("Load", "Load(): ProcessCmdMsg uspesne registrovany");
			return;
		}//*/
	}	

	fFunctionHandler = (FunctionHandler)GetProcAddress(hDll,"Function");
	if (fFunctionHandler) {
		CLog::WriteLog("Load", "Load(): Function");
		Functions.Register(fFunctionHandler);
		CLog::WriteLog("Load", "Load(): Function registered");
		return;
	}

	fSendingFunction = (SendMsgHandler)GetProcAddress(hDll,"SendingFunction");
	if (fSendingFunction) {
		SendingFunction.Register(fSendingFunction);
		return;
	}

	fProcessHandler = (ProcessHandler)GetProcAddress(hDll,"Process");
	if (fProcessHandler) 
	{
		fGetIdHandler = (GetNameHandler)GetProcAddress(hDll,"GetId"); //ak nema process meno ani ho neregistrujem
		if (fGetIdHandler) 
		{
			char* pId = new char [SIZE_NAME];
			size_t iSize=fGetIdHandler(pId, SIZE_NAME);
			if (iSize) {
				delete [] pId; pId=new char[iSize+1];
				fGetNameHandler(pId, iSize);
			}
			Processes.Register(fProcessHandler, pId);
			delete [] pId; 
			return;
		} //*/
	}
	return;
}

void LoadPlugins()
{
	CLog::WriteLog("LoadPlugins", "LoadPlugins(): Begin load plugins!");
  char   psBuffer[LONG_LINE];
  FILE * chkdsk;

  char * currentdir=_getdcwd(_getdrive(),NULL,0);
  /* Run DIR so that it writes its output to a pipe. Open this
   * pipe with read text attribute so that we can read it 
   * like a text file. 
   */
	wchar_t pwcPluginsFolder[MAX_PATH];
	swprintf(pwcPluginsFolder, L"%sPlugins\\", pwcProgramFolder);

  _wchdir(pwcPluginsFolder);
	CLog::WriteLog("LoadPlugins", "LoadPlugins(): Zmeneny adresar!");
  if( (chkdsk = _popen( "dir *.dll", "rt" )) == NULL )
    exit( 1 );
	CLog::WriteLog("LoadPlugins", "LoadPlugins(): Otvorena pipe!");
  /* Read pipe until end of file. End of file indicates that 
   * CHKDSK closed its standard out (probably meaning it 
   * terminated).
   */
  wchar_t dll[LONG_LINE];
  while( !feof( chkdsk ) )
  {
	  if( fgets( psBuffer, LONG_LINE, chkdsk ) != NULL ){
			switch(finddll(psBuffer, dll, LONG_LINE)) {
				case YES:
					CLog::WriteLog("LoadPlugins", "LoadPlugins(): Spustam funkciu Load()");
					Load(dll);
					break;
				default:;
			} 
		}	
  }
  /* Close pipe and print return value of CHKDSK. */
  _pclose( chkdsk );
  _chdir(currentdir);

  return;
}
 

CFunctionThread ** arrayFunctions=NULL;
CSendingFunctionThread ** arraySendingFunctions=NULL;

void StartPlugins()
{
	arrayFunctions = new CFunctionThread* [Functions.Size()];
	for (index i=0; i<Functions.Size(); ++i) {
		arrayFunctions[i] = new CFunctionThread(Functions[i]);
		arrayFunctions[i]->start();
	}

	arraySendingFunctions = new CSendingFunctionThread * [SendingFunction.Size()];
	for (index i=0; i<SendingFunction.Size(); ++i) {
		arraySendingFunctions[i] = new CSendingFunctionThread(SendingFunction[i], PushBackMessageToQueqeOut);
		arraySendingFunctions[i]->start();
	}
}

void StopPlugins()
{
	if (arrayFunctions) {
		for (index i=0; i<Functions.Size(); ++i) {
			arrayFunctions[i]->stop();
			delete arrayFunctions[i];
		}
		delete [] arrayFunctions;
	}		
	if (arraySendingFunctions) {
		for (index i=0; i<SendingFunction.Size(); ++i) {
			arraySendingFunctions[i]->stop();
			delete arraySendingFunctions[i];
		}
		delete [] arraySendingFunctions;
	}
}

