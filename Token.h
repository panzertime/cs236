
/*
*	Tokens have Value, Kind, and Line
*	These can be declared in the constructor
*	Print function outputs to a stream
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "Kind.h"

using namespace std;

class Token {
public:
	string Value;
	unsigned Line;
//	Kind tokenType;

	map<int, string> kinds;


//public:
	Kind tokenType;

	Token(string V, unsigned L, Kind K){
		Value = V;
		Line = L;
		tokenType = K;

		kinds[0] = "COMMA";
		kinds[1] = "PERIOD";
		kinds[2] = "Q_MARK";
		kinds[3] = "LEFT_PAREN";
		kinds[4] = "RIGHT_PAREN";
		kinds[5] = "COLON";
		kinds[6] = "COLON_DASH";
		kinds[7] = "SCHEMES";
		kinds[8] = "FACTS";
		kinds[9] = "RULES";
		kinds[10] = "QUERIES";
		kinds[11] = "ID";
		kinds[12] = "STRING";
	}

	virtual ~Token(){
	}
	
	void print(ofstream & out){
	out << "(" << kinds[tokenType] << ",\"" << Value << "\"," << Line
	<< ")" << endl;
	}

};

#endif

