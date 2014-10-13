#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "Parameter.h"

using namespace std;

class Predicate {

public:

string label;
vector<Parameter> params;


	Predicate(){

	}

	virtual ~Predicate(){

	}

	void addParam(Parameter p){
		params.push_back(p);
	}

	string toString(){
		string s;
		s += label;
		s += "(";
		for (auto t : params){
			s += t.toString();
			s += ",";
		}
		s.pop_back();
		s += ")";
	//	s += "\n";
		return s;
	}

};

#endif
