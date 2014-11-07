#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include <string>
#include "Scheme.h"
#include "Predicate.h"

class Relation {

public:

	Relation(Predicate & src){
		string Name = src.label;
		Scheme scheme = Scheme(src);
	}

	Relation(Scheme & src){
		Scheme scheme = src;
		string Name = "no name";
	}

	Relation(){
		// nothing... this is for the copy to work
		Scheme scheme;
		string Name;
	}

	virtual ~Relation(){
	}

	set<Tuple> tuples;
	string Name;
	Scheme scheme;

	void add(Tuple & t){
		tuples.insert(t);
	}

	Relation select(int & pos, string & val){
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

	Relation project(vector<int> & order){
		// go thru "order," use this to determine how 
		// attrs are copied to new relation
		//
		// kind of like make a new relation with
		// the right scheme, should be fairly easy
		// maybe new constructor for Scheme()?
		//
		// then go thru the tuples, make a new tuple based on
		// order, add to new relation
		
		vector<string> attrs;
		for(auto index : order){
			attrs.push_back(scheme.attrs[index]);
		}
		
		Scheme s = Scheme(attrs);
		Relation r = Relation(s);

		for(Tuple t : tuples){
			Tuple n;
			for(auto index : order){
				n.push_back(t[index]);
			}
			r.add(n);
		}
		
		return r;
	}

	Relation rename(vector<string> vars){
		Scheme ns = Scheme(vars);
		return Relation(ns);
	}

	string toString(){
		string r;
		r += Name += "\n";
		for (Tuple t : tuples){
			r += "  ";
			for (int i = 0; i < scheme.attrs.size(); i++){
				r += scheme.attrs[i] += "='";
				r += t[i] += "'\n";
			}
		}
		return r;
	}
};

#endif
