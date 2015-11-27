#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

using namespace std;

class Parameter{

public:

	bool ID;
	bool expr;
	string value;
	
	void addName(string s){
		ID = 1;
		expr = 0;
		value = s;
	}

	void addValue(string s){
		ID = 0;
		expr = 0;
		value = s;
	}

	bool inDomain(){
		return !(ID || expr);
		// if ID or expr is true, return false
		// did this to reduce pmccabe of DatalogProgram::createDomain,
		// it did nothing
	}

	virtual string toString(){
		return value;
	// virtual does not seem to be virtualizing, whatever,
	// this is why I hate C++
	// "let's do OOP in a language the inventor constantly
	//    is shouting is NOT OO!"
	// due to changes in how strings are handled:
	//	if(ID)
	//		return value;
	//	else
	//		return value;
	}

	Parameter(){

	}

	virtual ~Parameter(){

	}

};

#endif
