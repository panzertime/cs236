#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

class DatalogProgram {

private:
	vector<Predicate> Schemes;
	vector<Predicate> Facts;
	vector<Rule> Rules;
	vector<Predicate> Queries;
	set<string> Domain;


public:
	DatalogProgram(){

	// what to instantiate?
	
	}

	virtual ~DatalogProgram(){
	}

	void addScheme(Predicate p){
		Schemes.push_back(p);
	}

	void addFact(Predicate p){
		Facts.push_back(p);
	}

	void addRule(Rule r){
		Rules.push_back(r);
	}

	void addQuery(Predicate p){
		Queries.push_back(p);
	}

	void createDomain(){
		for(auto p : Facts){
			for(auto t : p.params){
				if(!t.ID)
					Domain.insert(t.value);
			}
		}
		for(auto p : Queries){
			for(auto t : p.params){
				if(!t.ID)
					Domain.insert(t.value);
			}
		}
	//	Domain.sort();
	}


	string toString(){
		string s;
		s += "Schemes(";
		s += to_string(Schemes.size());
		s += "):\n";
		for(auto p : Schemes){
			s += "  ";
			s += p.toString();
			s += "\n";
		}
		s += "Facts(";
		s += to_string(Facts.size());
		s += "):\n";
		for(auto p : Facts){
			s += "  ";
			s += p.toString();
			s += "\n";
		}
		s += "Rules(";
		s += to_string(Rules.size());
		s += "):\n";
		for(auto r : Rules){
			s += "  ";
			s += r.toString();
			s += "\n";
		}
		s += "Queries(";
		s += to_string(Queries.size());
		s += "):\n";
		for(auto p : Queries){
			s += "  ";
			s += p.toString();
			s += "\n";
		}
		s += "Domain(";
		s += to_string(Domain.size());
		s += "):\n";
		for(auto t : Domain){
			s += "  ";
			s += t;
			s += "\n";
		}

		return s;
	}
};

#endif
