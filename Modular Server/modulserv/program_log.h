// 19.11.2005 - hlavickovy subor pre log programu

#ifndef MY_PROGRAM_LOG
#define MY_PROGRAM_LOG

#include <windows.h>
#include <fstream>

class CLog 
{
private:
	BOOL m_bLocked;
	void * m_pLocker;
	char m_aLog[BUFSIZE];
	char pLogFile[MAX_PATH];

private:
	void Loging(const char *pData, int iLen);
	inline void Lock(void* pLocker) { while(m_bLocked) Sleep(TIMER); m_bLocked=TRUE; m_pLocker=pLocker; };
	inline void Unlock(void* pLocker) { if(pLocker==m_pLocker) m_bLocked=FALSE; };
	void Write(void* pLocker, const char* pData);
public:
	CLog() {m_bLocked=FALSE;};

	static CLog * GetInstance() 
	{
		static CLog * InstanceLog = new CLog; 
		return InstanceLog ;
	};
	static void WriteLog(void* pLocker, const char* pData);
};

static CLog * pCLogInstance = CLog::GetInstance();

inline void DeallocLogClass()
{
	if (pCLogInstance) delete pCLogInstance;
};

#endif