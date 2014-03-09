/** ou_thread.h
* declares the Thread interface and associated classes
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


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/* Pali Jancura - to jest ja :)
* 14.05.2005
*	- musel som spravit urcite upravy oproti povodnej versii, su poznamenane 
*	  v komentaroch, takto uz by to malo byt bez chybicky krasy :)
*/
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//doplnil som ochranu pred viacnasobnym vkladanim
#ifndef MY_OU_THREAD_H
#define MY_OU_THREAD_H

//tuto hlavicku si vyzaduje moj compiler Visual C++.Net, pre spravne chovanie 
//linkovania a spajania hlavickovych subor
#include "stdafx.h"

//v povodnej verzii je este jedna chyba, chyba vlozena kniznica pre podporu
//stringov, takze ju vkladam, plus doplnenie std
#include <string>

namespace my_openutils {
	class MyThreadException;
	class MyMutex;

	/** class Thread
	* Represents a thread of execution
	* in the process. To create a new Thread
	* write an inherited class ot Thread and
	* override the run() function
	**/
	class MyThread {

	private:
		// unsigned long* to the low-level thread object
		unsigned long* m_hMyThread;
		// a name to identify the thread
		std::string m_strName;
		bool m_bSuspend;

	public:
		MyThread();
		MyThread(const char* nm);
		~MyThread();
		void setName(const char* nm);
		std::string getName() const;
		void start();
		virtual void run();
		void sleep(long ms);
		void suspend1();
		void suspend2();
		void resume();
		//void resume2();
		void stop();
		void stop2();

		void setPriority(int p);

		bool wait(const wchar_t* m,long ms=5000);
		void release(const wchar_t* m);

	public:
		// Thread priorities
		static const int P_ABOVE_NORMAL;
		static const int P_BELOW_NORMAL;
		static const int P_HIGHEST;
		static const int P_IDLE;
		static const int P_LOWEST;
		static const int P_NORMAL;
		static const int P_CRITICAL;
	private:				
	};// class MyThread	

	/** class MyMutex
	* Represents a Mutex object to synchronize
	* access to shaed resources.
	**/
	class MyMutex {
	private:
		// unsigned long* to the low-level mutex object
		unsigned long* m_hMyMutex;
		// name to identify the mutex
		std::string m_strName;
	public:
		MyMutex();
		MyMutex(const wchar_t* nm);
		void create(const wchar_t* nm);
		unsigned long* getMyMutexHandle();
		std::string getName();
		void release();
		~MyMutex();
	};

	/** class ThreadException
	* thrown by Thread and Mutex function 
	* calls
	**/
	class MyThreadException {
	private:
		std::string msg;
	public:
		MyThreadException(const char* m);
		std::string getMessage() const;
	};	
} // namespace openutils

// global function called by the thread object.
// this in turn calls the overridden run()
extern "C" {
	unsigned int my_ou_MyThread_proc(void* param);
}

#endif
