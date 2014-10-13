#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include "Predicate.h"

using namespace std;

class Rule {

private:

	Predicate head;
	vector<Predicate> tail;

public:

	Rule(){

	}

	virtual ~Rule(){

	}

	string toString(){
		string s;
		s += head.toString();
		s += " :- ";
		for (auto p : tail){
			s += p.toString();
			s += ",";
		}
		s.pop_back();
		return s;
	}

	void addHead(Predicate p){
		head = p;
	}

	void addTail(Predicate p){
		tail.push_back(p);
	}


};

#endif
