//implemenatcia funkcie ktora nacita Message Codes z MessageCodes.txt a uchova si ich

#include "stdafx.h"
#include "Plugins.h"

#include <fstream>

//std::vector<int> vecCodes;

void LoadCodes(std::ifstream& CodeTxt)
{
	bool space=false; size_t i;
	std::string Code, Pom;
	while(getline(CodeTxt, Pom)){
		i=0;
		while(i != Pom.size() && isspace(Pom[i])) ++i;
		if(i == Pom.size()) continue;

		while(i < Pom.size()){
			if (!isspace(Pom[i])) {
				if (!space && atoi(Pom.c_str()+i)) Code.push_back(Pom[i]);
				else { break; Code=""; }
			} else space=true;
			++i;
		}
		
		if (!Code.empty()) vecCodes.push_back(atoi(Code.c_str()));
	}
}