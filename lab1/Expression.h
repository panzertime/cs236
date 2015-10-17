#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

using namespace std;

class Expression: public Parameter{

public:

	Parameter left;
	Parameter right;
	Kind oper;

	void addExpr(Parameter leftParam, Kind operatorType, Parameter rightParam){
		ID = 0;
		expr = 1;
		left = leftParam;
		oper = operatorType;
		right = rightParam;
	}

	string toString(){
		return "expression ";
		if(oper == ADD)
			return ( "(" + left.toString() + "+" + right.toString() + ")" );
		else
			return ( "(" + left.toString() + "*" + right.toString() + ")" );

	}

	Expression(){

	}

	virtual ~Expression(){

	}

};

#endif
