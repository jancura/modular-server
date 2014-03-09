//20.05.2005 - implementacia load_conf.cpp,sluzi pre nacitanie a konfiguracneho
//suboru do stringu

/*Pali Jancura - to jest ja :)
 * 20.05.2005
 *	add:	string loadconfig(ifstream& config)
 */

#include "stdafx.h"

#include "load_conf.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/******************************************************************************
 * string loadconfig(ifstream& config)
 *	- funkcia pre nacitanie configuracneho suboru,cely subor da do jedneho riadku
 ******************************************************************************/
string loadconfig(ifstream& config)
{
	string line, pom;
	while(getline(config, pom)){
		line+=pom;
		line.push_back('\n'); //getline neberie znak konca riadku
	}
	return line;
}//string loadconfig()