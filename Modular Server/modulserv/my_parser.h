//20.05.2005 - hlavickovy subor pre my_parser.cpp

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include "stdafx.h"

#include <string>

std::string word(const std::string&, std::string::size_type&);
bool my_parser(const std::string&);

#endif