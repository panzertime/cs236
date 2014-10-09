#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include "Predicate.h"
#include "Rule.h"
#include <vector.h>
#include <string>

using namespace std;

class DatalogProgram {

private:
	vector<Predicate> Schemes;
	vector<Predicate> Facts;
	vector<Rule> Rules;
	vector<Predicate> Queries;


public:
	DatalogProgram(){

	// what to instantiate?
	
	}

	virtual ~DatalogProgram(){
	}

	addScheme(Predicate p){
		Schemes.push_back(p);
	}

	addFact(Predicate p){
		Facts.push_back(p);
	}

	addRule(Rule r){
		Rules.push_back(r);
	}

	addQuery(Predicate p){
		Queries.push_back(p);
	}

	string toString(){
		string s;
		s += "Schemes(";
		s += Schemes.size();
		s += "):\n";
		for(auto p : Schemes){
			s += "  ";
			s += p.toString;
			s += "\n";
		}
		s += "Facts(";
		s += Facts.size();
		s += "):\n";
		for(auto p : Facts){
			s += "  ";
			s += p.toString;
			s += "\n";
		}
		s += "Rules(";
		s += Rules.size();
		s += "):\n";
		for(auto r : Rules){
			s += "  ";
			s += r.toString;
			s += "\n";
		}
		s += "Queries(";
		s += Queries.size();
		s += "):\n";
		for(auto p : Queries);
			s += "  ";
			s += p.toString;
			s += "\n";
		}

};

#endif
