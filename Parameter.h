#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

using namespace std;

class Parameter{

public:

	bool ID;
	string value;
	
	void addName(string s){
		ID = 1;
		value = s;
	}

	void addValue(string s){
		ID = 0;
		value = s;
	}

	string toString(){
		if(ID)
			return value;
		else
			return "'" + value + "'";
	}

	Parameter(){

	}

	virtual ~Parameter(){

	}

};

#endif
