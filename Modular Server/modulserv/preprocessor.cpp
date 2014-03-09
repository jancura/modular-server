//20.05.2005 - implementacia preprocessor.cpp, sluzi na spracovnie retazca
//do podla mnou standardizovaneho retazca

/*Pali Jancura - to jest ja :)
 * 20.05.2005
 *	add:	string preprocessor(const string& retazec) - funkcny
 */

#include "stdafx.h"

#include "preprocessor.h"
#include "program_log.h"

#include <iostream>
#include <cctype>
#include <string>

using namespace std;

/******************************************************************************
 * string preprocessor(const string& retazec)
 *	- sluzi na spracovanie akehokolvek retazca do mnou standardizovanej podoby
 *	pre parser
 *****************************************************************************/
string preprocessor(const string& retazec)
{
	string standard;
	string::size_type i = 0;
	bool rovnitko = false; //ak uz som nasiel rovnitko, tak true
	bool token = false; //signalizuje, ci uz mam jeden token, '=' rozdeluje na
	                    //dva tokeny, viac tokenov nie je pripustne
	bool space = false; //ci som nasiel biely znak
	bool znak = false;  //bude siganlizovat ci v tokene je znak, ak nie tak
					    //chybna konfiguracia,tokeny nemozu byt prazdne
	bool newline = false; //signalizuje, ci nie sme na novom riadku

	//preskocim prve biele znaky po prvy znak
	while(i != retazec.size() && isspace(retazec[i])) ++i;

	if(i == retazec.size()){
		CLog::WriteLog("preprocessor", "preprocessor(): Prazdny konfigurak!");
		return "mistake";
	}

	//dokial neprejdem cely retazec, tak ho rozoberam a skladam novy 
   while(i < retazec.size()){
		if(retazec[i] == ';')
		{
			//ci nebol hned prvy znak od rovnitka ';', potom nema pravu stranu
			if(rovnitko){
				CLog::WriteLog("preprocessor", "preprocessor(): Rovnitko bez pravej strany!");
				return "mistake";
			}
			//je to komentar,preskocim to
			while(i < retazec.size() && retazec[i] != '\n') ++i;
			//ak je komentar za nastavenim,musim ukoncit znakom '\n', oddelovac
			if(standard.size() != 0 && *(standard.end()-1) != '\n'){
				standard.push_back('\n');
				newline = true;		//vlozili sme novy riadok
				rovnitko = false;	//zvysne premenne nastavim defaultne
				space = false;		//predchadzajuci riadok bol korektny
				znak = false;
				token = false;
            }
			++i;
		}
		else
		{
			if(!isspace(retazec[i])){
				znak = true; //je to znak
				//ak sme nevkladali novy riadok a posledne znaky boli biele, tak
				//ich nahradim jednym bielym znakom ' '
				if(space && !newline) standard.push_back(' ');
			
				//dohliadnem na medzery okolo '=' (left=right => left = right)
				if(retazec[i] == '=' && !space) standard.push_back(' ');
				if(i > 0 && retazec[i-1] == '=' && !space)
					standard.push_back(' ');
				//teraz uz mozem zneplatnit space,kedze nacitanym znakom je znak
				space = false;
				//kontrolujem spravnost zapisu okolo '=', musi mat obidve strany
				//definovane a nesmie byt viac ako jedno '=' v jednom riadku
				if(retazec[i] == '='){
					//ak sme vlozili novy riadok a hned prvy znak je '='
					//vtedy este musi byt token = false, urcite nema lavu stranu
					if(newline && !token){
						CLog::WriteLog("preprocessor", "preprocessor(): Rovnitko bez lavej strany!");
						return "mistake";
					}
					//poznamename ze je to rovnitko a nastavime token
					if(!token){ rovnitko = true; token = true;} 
					else {//jedno rovnitko uz je,nasli sme druhe v jednom riadku
						CLog::WriteLog("preprocessor", "preprocessor(): Dva rovnitka v jednom riadku");
						return "mistake";
					}
				}else if(rovnitko) rovnitko = false;
				//ak sme na novom riadku a nacitali sme znak,nastavim pre 
				//pripad dalsieho noveho riadku
				if(newline) newline = false;
				
				standard.push_back(retazec[i]);
				++i;
			}else{
				//je to biely znak
				if(!space) space = true;
				//preskocim biele znaky
				while(i < retazec.size() && isspace(retazec[i])){
					//ak medzi nimi bol novy riadok,poznamenam si to
					if(retazec[i] == '\n'&& !newline) newline = true;
					++i;
				}
				//kontrola spravnosti konfiguracie, ak sme presli na novy 
				//riadok a rovnitko ostalo bez pravej strany
				if(newline && rovnitko){
					CLog::WriteLog("preprocessor", "preprocessor(): Rovnitko bez pravej strany!");
					return "mistake";
				}
				//ak bol na predchadzajucom riadku znak a nie komentar, tak
				//vlozim novy riadok
				if(newline && znak){
					standard.push_back('\n');
					space = false;		//zacnem citat novy riadok,zvysne
					rovnitko = false;	//premenne nastavim defaultne
					token = false;		//predchadzajuci riadok bol korektny
				}
				znak = false;//posledny znak bol biely znak,mozem zneplatnit
			}//if(!isspace(retazec[i]))
		}//if(retazec[i] == ';')
	}//while

	CLog::WriteLog("preprocessor", "preprocessor.cpp: preprocessor allright");

	return standard;
}