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

	virtual string toString(){
		return value;
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
