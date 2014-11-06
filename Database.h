#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include "Relation.h"
#include "Scheme.h"
#include "DatalogProgram.h"
#include "Predicate.h"

class Database {

public:

	Database(DatalogProgram & src){
		for(Predicate P : src.Schemes){
			addRelation(P);
		}	// adding all the schemes as relations
		for(Predicate P : src.Facts){
			Tuple t;
			for(Parameter m : P.params){
				t.push_back(m.value);
			}
			relations[P.label].add(t);
		}	// adding tuples to relations
		//
		// sort the map?  if so, how exactly?
		// sort the tuples?  if so, how exactly?
		// 
	}

	virtual ~Database(){
	}

	map<string,Relation> relations;

	void addRelation(Predicate & src){
		relations[src.label] = Relation(src);
	}
	
	string queryEval(Predicate & q) {
		Relation src = relations[q.label];
		Relation projected;
		Relation renamed;
		for (Parameter s : w.params){
			if(param.ID){
				// select variable
			}
			if(!(param.ID)){
				// select constant
			}
		}
		vector<int> cols = ;
			// figure out how to project exactly
		projected = src.project(cols);
		vector<string> vars = ;
			// figure out how to rename exactly
		renamed = src.rename(vars);
		
		if (projected.tuples.size() == 0){
			return ("No\n");
		}
		else {
			string r;
			r += "Yes(" += projected.tuples.size() += ")\n";
			r += src.toString();
			r += projected.toString();
			r += renamed.toString();

			return r;
		}

		// make a string, formatted as directed, 
		// return it - it will += to the output
	}	

	// to add a tuple to a relation, do
	// DB.relations[name].add(tuple & t);
	// this should work: relations[name]
	// gives the right relation, .add() 
	// is in Relation.h and inserts a tuple
	// to the Relation's set

private:


};


#endif 
