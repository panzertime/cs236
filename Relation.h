#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include <string>
#include "Scheme.h"
#include "Predicate.h"

class Relation {

public:

string Name;
Scheme scheme;

	Relation(Predicate & src){
		Name = src.label;
		scheme = Scheme(src);
	}

	Relation(Scheme & src){
		scheme = src;
		Name = "no name";
	}

	Relation(){
		// nothing... this is for the copy to work
		
	
	}

	virtual ~Relation(){
	}

	set<Tuple> tuples;

	void add(Tuple & t){
		tuples.insert(t);
	}

	Relation select(int & pos, string & val){
		Relation r = Relation(scheme);
		for(Tuple t : tuples){
			if (t[pos] == val)				
				r.add(t);
		}
		return r;
	}

	Relation select(int & pos1, int & pos2){
		Relation r = Relation(scheme);
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
		for(int i = 0; i < (int) order.size(); i++){
			int index = order[i];
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
		Relation r = Relation(ns);
		r.tuples = tuples;
		return r;
	}

	string toString(){
		string r;
//		r += Name += "\n";
		if(!tuples.empty()){
			for (Tuple t : tuples){
				r += "  ";
				for (int i = 0; i < (int) scheme.attrs.size(); i++){
					r += scheme.attrs[i];
					r += "='";
			/* 	Somehow this line:
					r += scheme.attrs[i] += "='";
				was outputting ridiculous lines where
				the number of =' was equal to the i+1.
				NO IDEA how or why, but separating
				those two fixed it.
				Man, C++ += is a weird beast;
			*/
					r += t[i] += "' ";
				}
//				if(scheme.attrs.empty()){
//					r += "EMPTY SCHEME";
//				}
				r += "\n";
			}
		}
		return r;
	}
};

#endif
