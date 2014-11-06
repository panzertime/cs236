#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include <string>
#include "Scheme.h"

class Relation {

public:

	Relation(){
	}

	virtual ~Relation(){
	}

	set<Tuple> tuples;
	string Name;
	Scheme scheme;

	void add(tuple & t){
		tuples.insert(t);
	}

	Relation select(int pos, string & val){
		Relation r;
		for(tuple t : tuples){
			if t[pos] == val
				r.add(t);
		}
		return r;
	}

	Relation select(int & pos1, int & pos2){
		Relation r;
		for(tuple t : tuples){
			if t[pos1] == t[pos2]
				r.add(t);
		}
		return r;
	}

	Relation project(vector<int> order){
		// go thru "order," use this to determine how 
		// attrs are copied to new relation
	}

	Relation rename(/*  ?  */){
		// what
	}

};

#endif
