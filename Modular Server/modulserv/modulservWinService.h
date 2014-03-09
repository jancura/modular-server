#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::ServiceProcess;
using namespace System::ComponentModel;


/*****************************************************************************/
/*!!!!!!!  		 Moje funkcie a potrebne hlavicky, ci definicie				!!!!!!!!!*/ 
/*****************************************************************************/
#include "preprocessor.h"
#include "load_conf.h"
#include "my_parser.h"
#include "Plugins.h"

#include <winsock.h>
#include <fstream>
#include <ios>
#include <string>

using namespace std;

typedef size_t (*loadconf_PTR)(char*,size_t); 
loadconf_PTR loadconf = NULL;

wchar_t * pwcProgramFolder=NULL;
char *		pcProgramFolder =NULL;

inline bool ImportFromDLL(const wchar_t * pFolder)
{
	wchar_t pwcLibraryFolder[MAX_PATH];
	swprintf(pwcLibraryFolder, L"%sLibrary\\loadconfig.dll", pFolder);
	HINSTANCE hDll = LoadLibrary(pwcLibraryFolder);
	if (hDll == NULL) return false;
	loadconf = (loadconf_PTR)GetProcAddress(hDll,"load");
	return true;
};

inline wchar_t * GetApplicationFolder()
{
	wchar_t pwcPath[MAX_PATH]; 
	if( !GetModuleFileName(GetModuleHandle(NULL), pwcPath, MAX_PATH) ) {
		return NULL;
	}
	if (pwcPath)
	{
		int i=0; for (i=wcslen(pwcPath); (i>-1 && pwcPath[i]!= L'\\'); --i) {}
		pwcPath[++i]=L'\0';

		wchar_t * pwcApplicationPath = new wchar_t [wcslen(pwcPath)+1];
		swprintf(pwcApplicationPath, L"%s", pwcPath);
		return pwcApplicationPath;
	}
	else return NULL;
};
/*****************************************************************************/
/*///////////////						       END												///////////////*/						
/*****************************************************************************/


namespace modulserv {

	/// <summary>
	/// Summary for modulservWinService
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class modulservWinService : public System::ServiceProcess::ServiceBase
	{
	public:
		modulservWinService()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~modulservWinService()
		{
			if (components)
			{
				delete components;
			}
		}

		/// <summary>
		/// Set things in motion so your service can do its work.
		/// </summary>
		virtual void OnStart(array<String^>^ args) override
		{
			WSADATA data;
			WORD wVersionRequired = MAKEWORD(2,2);
			if(WSAStartup(wVersionRequired, &data) != 0){
				CLog::WriteLog("OnStart", "OnStart(): Initialization of socket was unsuccessful!");
				return;
			}

			string config;

			pwcProgramFolder=GetApplicationFolder();
			pcProgramFolder=ConvertWChar_tToCHAR(pwcProgramFolder);

			if (ImportFromDLL(pwcProgramFolder)) 
			{
				if (loadconf != NULL)
				{
					char * cp = new char[500];
					int len = loadconf(cp,500); 
					if (len>500) 
					{
						delete [] cp; cp = new char[len+1];
						loadconf(cp,len+1);
					}
					config = cp; delete [] cp;
					config = preprocessor(config);
				}
				else
				{
					CLog::WriteLog("OnStart","OnStart(): Unreachable function loadconf() in DLL!");
					return;
				}
			}
			else
			{
				CLog::WriteLog("OnStart", "OnStart(): Default way of configuration!");

				char pConfigFile[MAX_PATH];
				sprintf(pConfigFile, "%sserver.cfg", pcProgramFolder);
				ifstream vstup;
				vstup.open(pConfigFile, ios::in); //stream for default configuration

				config = preprocessor(loadconfig(vstup));

				if (vstup.is_open()) vstup.close();
			}

			char log[BUFSIZE];
			sprintf(log, "OnStart(): \n%s", config.c_str());
			CLog::WriteLog("OnStart", log);

			LoadPlugins();
			CLog::WriteLog("OnStart", "OnStart(): Nacitane pluginy!");
			StartPlugins();
			CLog::WriteLog("OnStart", "OnStart(): Spustene pluginy!");
			my_parser(config);
		}

		/// <summary>
		/// Stop this service.
		/// </summary>
		virtual void OnStop() override
		{
			sID Sender; Sender.type=T_PCHAR; strcpy(Sender.t_name,"OnShutdown");
			sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"ALL");
			CMessageQueqe::PushBackMessageToQueqe(Sender, Receiver, END);

			StopProcesses();
			StopPlugins();
			WSACleanup();

			CLog::WriteLog("OnStop", "OnStop(): Server is closed!");

			//dealloc
			if (pwcProgramFolder) delete [] pwcProgramFolder;
			if (pcProgramFolder)	delete [] pcProgramFolder;

			DeallocMessageQueqe();
			DeallocLogClass();
		}

		virtual void OnShutdown() override
		{
			sID Sender; Sender.type=T_PCHAR; strcpy(Sender.t_name,"OnStop");
			sID Receiver; Receiver.type=T_PCHAR; strcpy(Receiver.t_name,"ALL");
			CMessageQueqe::PushBackMessageToQueqe(Sender, Receiver, END);

			StopProcesses();
			StopPlugins();
			WSACleanup();

			CLog::WriteLog("OnShutdown", "OnShutdown(): Server is closed!");
			
			//dealloc
			if (pwcProgramFolder) delete [] pwcProgramFolder;
			if (pcProgramFolder)	delete [] pcProgramFolder;

			DeallocMessageQueqe();
			DeallocLogClass();
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			// 
			// modulservWinService
			// 
			this->AutoLog = false;
			this->CanShutdown = true;
			this->ServiceName = L"Modular Server 1.0";

		}
#pragma endregion
	};
}
