#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "Parameter.h"
#include "Expression.h"

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
		for (int i = 0; i < params.size(); i++) {
			s += (&(params[i]))->toString();
			s += ",";
		}

/**		for (auto t : params){
			Parameter* paramPtr;
			Expression* exprPtr;
			if (t.expr == 1){
				exprPtr = &t;
				s += exprPtr->toString();
			}
			else {
				paramPtr = &t;
				s += paramPtr->toString();
			}
		//	s += "EXPRESSION ";
		//	}
		//	else {
		//	s += t.toString();
			s += ",";
		//}    **/
		
		s.pop_back();
		s += ")";
	//	s += "\n";
		return s;
	}

};

#endif
