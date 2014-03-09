/** ou_MyThread.cpp
* implements the MyThread class
* Author: Vijay Mathew Pandyalakal
* Date: 13-OCT-2003
**/

/* Copyright 2000 - 2005 Vijay Mathew Pandyalakal.  All rights reserved.
*
* This software may be used or modified for any purpose, personal or
* commercial.  Open Source redistributions are permitted.  
*
* Redistributions qualify as "Open Source" under  one of the following terms:
*   
*    Redistributions are made at no charge beyond the reasonable cost of
*    materials and delivery.
*
*    Redistributions are accompanied by a copy of the Source Code or by an
*    irrevocable offer to provide a copy of the Source Code for up to three
*    years at the cost of materials and delivery.  Such redistributions
*    must allow further use, modification, and redistribution of the Source
*    Code under substantially the same terms as this license.
*
* Redistributions of source code must retain the copyright notices as they
* appear in each source code file, these license terms, and the
* disclaimer/limitation of liability set forth as paragraph 6 below.
*
* Redistributions in binary form must reproduce this Copyright Notice,
* these license terms, and the disclaimer/limitation of liability set
* forth as paragraph 6 below, in the documentation and/or other materials
* provided with the distribution.
*
* The Software is provided on an "AS IS" basis.  No warranty is
* provided that the Software is free of defects, or fit for a
* particular purpose.  
*
* Limitation of Liability. The Author shall not be liable
* for any damages suffered by the Licensee or any third party resulting
* from use of the Software.
*/


//tuto hlavicku si vyzaduje moj compiler Visual C++.Net, pre spravne chovanie 
//linkovania a spajania hlavickovych subor, rozdiel oproti povodnemu
#include "stdafx.h"

#include <string>
using namespace std;

//#include <windows.h>

#include "my_ou_thread.h"
using namespace my_openutils;

const int MyThread::P_ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL;
const int MyThread::P_BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL;
const int MyThread::P_HIGHEST = THREAD_PRIORITY_HIGHEST;
const int MyThread::P_IDLE = THREAD_PRIORITY_IDLE;
const int MyThread::P_LOWEST = THREAD_PRIORITY_LOWEST;
const int MyThread::P_NORMAL = THREAD_PRIORITY_NORMAL;
const int MyThread::P_CRITICAL = THREAD_PRIORITY_TIME_CRITICAL;

/**@ The MyThread class implementation
**@/

/** MyThread()
* default constructor
**/  
MyThread::MyThread() 
: m_bSuspend(false)
{
	m_hMyThread = NULL;
	m_strName = "null";
}

/** MyThread(const char* nm)
* overloaded constructor
* creates a MyThread object identified by "nm"
**/  
MyThread::MyThread(const char* nm) 
: m_bSuspend(false)
{
	m_hMyThread = NULL;
	m_strName = nm;
}

MyThread::~MyThread() {
	if(m_hMyThread != NULL) {
		stop();
	}
}

/** setName(const char* nm)
* sets the MyThread object's name to "nm"
**/  
void MyThread::setName(const char* nm) {	
	m_strName = nm;
}

/** getName()
* return the MyThread object's name as a string
**/  
string MyThread::getName() const {	
	return m_strName;
}

/** run()
* called by the MyThread callback my_ou_MyThread_proc()
* to be overridden by child classes of MyThread
**/ 
void MyThread::run() {
	// Base run
}

/** sleep(long ms)
* holds back the MyThread's execution for
* "ms" milliseconds
**/ 
void MyThread::sleep(long ms) {
	Sleep(ms);
}

/** start()
* creates a low-level MyThread object and calls the
* run() function
**/ 
void MyThread::start() {
	DWORD tid = 0;	
	m_hMyThread = (unsigned long*)CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)my_ou_MyThread_proc,(MyThread*)this,0,&tid);
	if(m_hMyThread == NULL) {
		throw MyThreadException("Failed to create MyThread");
	}else {
		setPriority(MyThread::P_NORMAL);
	}
}

/** stop()
* stops the running MyThread and frees the MyThread handle
**/ 
void MyThread::stop()
{
	if(m_hMyThread == NULL) return;	
	//		WaitForSingleObject(m_hMyThread,INFINITE);  //ochrana proti zacykleniu
	CloseHandle(m_hMyThread);
	m_hMyThread = NULL;
}

void MyThread::stop2()
{
	if(m_hMyThread == NULL) return;	
	WaitForSingleObject(m_hMyThread,INFINITE);  //ochrana proti zacykleniu
	CloseHandle(m_hMyThread);
	m_hMyThread = NULL;
}

/** setPriority(int tp)
* sets the priority of the MyThread to "tp"
* "tp" must be a valid priority defined in the
* MyThread class
**/ 
void MyThread::setPriority(int tp) {
	if(m_hMyThread == NULL) {
		throw MyThreadException("MyThread object is null");
	}else {
		if(SetThreadPriority(m_hMyThread,tp) == 0) {
			throw MyThreadException("Failed to set priority");
		}
	}
}

/** suspend()  
* suspends the MyThread
**/
void MyThread::suspend1()
{
	if(m_hMyThread == NULL) {
		throw MyThreadException("MyThread object is null");
	}else {
		if(SuspendThread(m_hMyThread) < 0) {
			throw MyThreadException("Failed to suspend MyThread");
		}
		m_bSuspend=true; 
	}
}

void MyThread::suspend2()
{
	if(m_hMyThread == NULL) {
		throw MyThreadException("MyThread object is null");
	}else {
		WaitForSingleObject(m_hMyThread, INFINITE); //cakam, dokial sa nevykona run() cely, potom suspendujem
		if(SuspendThread(m_hMyThread) < 0) {
			throw MyThreadException("Failed to suspend MyThread");
		}
		m_bSuspend=true; 
	}
}

/** resume()  
* resumes a suspended MyThread
**/ 
/*void MyThread::resume1() {
	if(m_hMyThread == NULL) {
		throw MyThreadException("MyThread object is null");
	}else {
		if(ResumeThread(m_hMyThread) < 0) {
			throw MyThreadException("Failed to resume MyThread");
		}
	}
}//*/

void MyThread::resume() 
{
	if(m_bSuspend){
		if(m_hMyThread == NULL) {
			throw MyThreadException("MyThread object is null");
		}else {
			if(ResumeThread(m_hMyThread) < 0) {
				throw MyThreadException("Failed to resume MyThread");
				m_bSuspend=false;
			}
		}
	}
}

/** wait(const char* m,long ms)  
* makes the MyThread suspend execution until the
* MyMutex represented by "m" is released by another MyThread.
* "ms" specifies a time-out for the wait operation.
* "ms" defaults to 5000 milli-seconds
**/ 
bool MyThread::wait(const wchar_t* m,long ms) {
	HANDLE h = OpenMutex(MUTEX_ALL_ACCESS,FALSE,m);
	if(h == NULL) {
		throw MyThreadException("MyMutex not found");
	}
	DWORD d = WaitForSingleObject(h,ms);
	switch(d) {
	case WAIT_ABANDONED:
		throw MyThreadException("MyMutex not signaled");
		break;
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		throw MyThreadException("Wait timed out");
		break;
	}
	return false;
}

/** release(const char* m)  
* releases the MyMutex "m" and makes it 
* available for other MyThreads
**/ 
void MyThread::release(const wchar_t * m) {
	HANDLE h = OpenMutex(MUTEX_ALL_ACCESS,FALSE,m);
	if(h == NULL) {
		throw MyThreadException("Invalid MyMutex handle");
	}
	if(ReleaseMutex(h) == 0) {
		throw MyThreadException("Failed to release MyMutex");
	}
}

/**@ The MyMutex class implementation
**@/

/** MyMutex()
* default constructor
**/  
MyMutex::MyMutex() {
	m_hMyMutex = NULL;
	m_strName = "";
}

/** MyMutex(const char* nm)
* overloaded constructor
* creates a MyMutex object identified by "nm"
**/  
MyMutex::MyMutex(const wchar_t* nm) {	
	char * name = ConvertWChar_tToCHAR(nm);
	if (name) {
		m_strName = name;	
		delete [] name;
	} 
	m_hMyMutex = (unsigned long*)CreateMutex(NULL,FALSE,nm);
	if(m_hMyMutex == NULL) {
		throw MyThreadException("Failed to create MyMutex");
	}
}

/** create(const char* nm)
* frees the current MyMutex handle.
* creates a MyMutex object identified by "nm"
**/  
void MyMutex::create(const wchar_t * nm) {
	if(m_hMyMutex != NULL) {
		CloseHandle(m_hMyMutex);
		m_hMyMutex = NULL;
	}
	char * name = ConvertWChar_tToCHAR(nm);
	if (name) {
		m_strName = name;	
		delete [] name;
	} 
	m_hMyMutex = (unsigned long*)CreateMutex(NULL,FALSE,nm);
	if(m_hMyMutex == NULL) {
		throw MyThreadException("Failed to create MyMutex");
	}
}
/** getMyMutexHandle()
* returns the handle of the low-level MyMutex object
**/  
unsigned long* MyMutex::getMyMutexHandle() {
	return m_hMyMutex;
}

/** getName()
* returns the name of the MyMutex
**/ 
string MyMutex::getName() {
	return m_strName;
}

void MyMutex::release() {
	if(m_hMyMutex != NULL) {
		CloseHandle(m_hMyMutex);
	}
}

MyMutex::~MyMutex() {
	/*if(m_hMyMutex != NULL) {
	CloseHandle(m_hMyMutex);
	}*/
}

// MyThreadException
MyThreadException::MyThreadException(const char* m) {
	msg = m;
}

string MyThreadException::getMessage() const {
	return msg;
}

// global MyThread caallback
unsigned int my_ou_MyThread_proc(void* param) {
	MyThread* tp = (MyThread*)param;
	tp->run();
	return 0;
}