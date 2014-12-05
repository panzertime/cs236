#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include <string>
#include "Scheme.h"
#include "Predicate.h"
#include <algorithm>

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

	void add(const Tuple & t){
		tuples.insert(t);
	}

	bool joinable(const Tuple & t1, const Tuple & t2, Scheme & s1, Scheme & s2){
		for(unsigned outer = 0; outer < t1.size(); outer++){
			for(unsigned inner = 0; inner < t2.size(); inner++){
				if(s1.attrs[outer] == s2.attrs[inner] && t1[outer] != t2[inner])
					return false;
			}
		}
		return true;
	}
	
	Tuple makeTuple(const Tuple & t1, const Tuple & t2,Scheme & s1, Scheme & s2){
		//idk, but this is actually what joining is
		Tuple result = t1;
		for (auto v2 : t2){
			for (auto n2: s2.attrs){
				if(count(s1.attrs.begin(),s1.attrs.end(),n2))
					result.push_back(v2);
			}
		}
		return result;
	}

	Relation join(Relation & r2){
		Scheme ns = scheme.makeScheme(r2.scheme);
		Relation result = Relation(ns);
		for (set<Tuple>::iterator thist = tuples.begin(); thist != tuples.end(); thist++){
			for (set<Tuple>::iterator thatt = r2.tuples.begin(); thatt != r2.tuples.end(); thatt++){
			//	if(joinable(tuples[thist], r2.tuples[thatt], scheme, r2.scheme){
			//		Tuple nt = makeTuple(tuples[thist] ,r2.tuples[thatt], scheme, r2.scheme);
				if(joinable(*thist, *thatt, scheme, r2.scheme)){
					Tuple nt = makeTuple(*thist, *thatt, scheme, r2.scheme);
					result.add(nt);
				}
			}
		}
		return result;
	}

	void unionWith(Relation & r2){
	//	assert(scheme == r2.scheme);
		//add all tuples from r2 to this.tuples
		for (set<Tuple>::iterator t = r2.tuples.begin(); t != r2.tuples.end(); t++){
			add(*t);
		}
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
		Relation r;
		r.tuples = tuples;
		r.scheme = ns;
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
