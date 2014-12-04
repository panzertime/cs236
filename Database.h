#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include "Relation.h"
#include "Scheme.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include <sstream>
#include <assert.h>

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

	int size(){
		int count = 0;
		if (relations.empty()){
			return 0;
		}
		for(map<string,Relation>::iterator i = relations.begin(); i != relations.end(); i++){
			count += i->second.tuples.size();
			assert(count >= 0);
		}
		return count;
	}

	map<string,Relation> relations;

	string toString(){
		string s;
		s += "Scheme Evaluation\n\n";
		s += "Fact Evaluation\n\n";
		for (map<string,Relation>::iterator i = relations.begin(); i != relations.end(); i++){
			s += i->second.Name += "\n";
			s += i->second.toString();
		}
		return s;
	}

	void addRelation(Predicate & src){
		relations[src.label] = Relation(src);
	}

	void selection(Relation & r, Predicate & q){
		for (int i = 0; i < (int) q.params.size(); i++){
			if(q.params[i].ID){
				for(int other = i; other < (int) q.params.size(); other++){
					if (q.params[other].ID && (q.params[other].value == q.params[i].value))
							r = r.select(i,other);
				}
			}
			if(!(q.params[i].ID)){
				r = r.select(i, q.params[i].value);
			}
		}
	}

	
	Relation predPrep(Predicate & q) {
		Relation src = relations[q.label];
		// either a copy constructor is needed in
		// Relation.h or we need to be careful that
		// q.label points to an extant relation
		//
		// somehow a segfault, presumably here,
		// is killing the execution before even printing 
		// the DB.

		Relation projected;
		Relation renamed;
		selection(src, q);
		vector<int> cols;
		for (int i = 0; i < (int) q.params.size(); i++){
			// if param is var, then add param # to cols
			// somehow avoid doubles
		
			if(q.params[i].ID)
				cols.push_back(i);
			for (int k = 0; k < i; k++){
				if(q.params[i].value == q.params[k].value)
					cols.pop_back();
			}	// this is checking if i've already added that var,
				// then deleting it from end if I have.
				// "slow" but should work well.
		}
		projected = src.project(cols);
		vector<string> vars;
		for (int i = 0; i < (int) cols.size(); i++){
			int index = cols[i];
			vars.push_back(q.params[index].value);
		}
			// figure out how to rename exactly
		renamed = projected.rename(vars);
		
		return renamed;
	}	
	
	string queryEval(Predicate & q) {
		Relation src = relations[q.label];
	// either a copy constructor is needed in
	// Relation.h or we need to be careful that
	// q.label points to an extant relation
	//
	// somehow a segfault, presumably here,
	// is killing the execution before even printing 
	// the DB.

		Relation projected;
		Relation renamed;
		selection(src, q);
		vector<int> cols;
		for (int i = 0; i < (int) q.params.size(); i++){
			// if param is var, then add param # to cols
			// somehow avoid doubles
			
			if(q.params[i].ID)
				cols.push_back(i);
			for (int k = 0; k < i; k++){
				if(q.params[i].value == q.params[k].value)
					cols.pop_back();
			}	// this is checking if i've already added that var,
				// then deleting it from end if I have.
				// "slow" but should work well.
		}

		projected = src.project(cols);
		vector<string> vars;
		for (int i = 0; i < (int) cols.size(); i++){
			int index = cols[i];
			vars.push_back(q.params[index].value);
		}
			// figure out how to rename exactly
		renamed = projected.rename(vars);
		
		if (src.tuples.size() == 0){
			return ("No\n");
		}
		else {
			string r;
			r += "Yes(";
			stringstream size;
			size << src.tuples.size();
			r += size.str();
			r += ")\nselect\n";
			r += src.toString();
			r += "project\n";
			r += projected.toString();
			r += "rename\n";
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
