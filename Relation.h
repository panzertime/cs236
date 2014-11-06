#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include <string>
#include "Scheme.h"
#include "Predicate.h"

class Relation {

public:

	Relation(Predicate & src){
		Name = src.label;
		scheme = Scheme(src);
	}

	virtual ~Relation(){
	}

	set<Tuple> tuples;
	string Name;
	Scheme scheme;

	void add(Tuple & t){
		tuples.insert(t);
	}

	Relation select(int pos, string & val){
		Relation r;
		for(Tuple t : tuples){
			if (t[pos] == val)				
				r.add(t);
		}
		return r;
	}

	Relation select(int & pos1, int & pos2){
		Relation r;
		for(Tuple t : tuples){
			if (t[pos1] == t[pos2])
				r.add(t);
		}
		return r;
	}

	Relation project(vector<int> order){
		// go thru "order," use this to determine how 
		// attrs are copied to new relation
	}

	Relation rename(vector<string> vars){
		// translate scheme of *this* to a new scheme
		// given in a query
	}

	string toString(){
		string r;
		r += Name += "\n";
		for (Tuple t : tuples){
			r += "  ";
			for (int i = 0; i < scheme.attrs.size(); i++){
				r += scheme.attrs[i] += "='" += t[i] += "'\n";
			}
		}
		return r;
	}

#endif
