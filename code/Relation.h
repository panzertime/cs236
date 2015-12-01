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
		for(unsigned first = 0; first < t1.size(); first++){
			for(unsigned second = 0; second < t2.size(); second++){
				if(s1.attrs[first] == s2.attrs[second]){
					if(t1[first] != t2[second]){
						return false;
					}
				}
			}
		}
		return true;
	}
	
	Tuple makeTuple(const Tuple & t1, const Tuple & t2,Scheme & s1, Scheme & s2){
		//idk, but this is actually what joining is
		Tuple result = t1;
//		for (auto v2 : t2){
//			for (auto n2: s2.attrs){
//				if(count(s1.attrs.begin(),s1.attrs.end(),n2))
//					result.push_back(v2);
//			}
//		}
		for (int index = 0; index < (int) t2.size(); index ++){
			if(count(s1.attrs.begin(),s1.attrs.end(), s2.attrs[index]) == 0){
				result.push_back(t2[index]);
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
//cout << "New tuple: " << endl;
//for (auto qoi : nt){
//cout << qoi << " - ";
//}
//cout << endl;
					result.add(nt);
				}
			}
		}
//cout << "As seen by the program or w/e: " << endl;
//for (set<Tuple>::iterator thist = result.tuples.begin(); thist != result.tuples.end(); thist++){
//for (auto qu : *thist){
//cout << qu << " - ";
//}
//cout << endl;
//}
//cout << "But toString(); sees it differently, guess from the scheme: " << endl;
//cout << result.toString();
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
			if (t[pos] == val){
//cout << "ADDING SELECTED BY VAL MATCH TO NEW TUPLE" << endl;
				r.add(t);}
		}
		return r;
	}

	Relation select(int & pos1, int & pos2){
		Relation r = Relation(scheme);
		for(Tuple t : tuples){
			if (t[pos1] == t[pos2]){
//cout << "ADDING SELECTED BY TUPLE MATCH TO NEW TUPLE" << endl;
				r.add(t);}
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
//cout << "PROJECTION: MAKING SCHEME" << endl;
			int index = order[i];
			attrs.push_back(scheme.attrs[index]);
		}
		
		Scheme s = Scheme(attrs);
		Relation r = Relation(s);

		for(Tuple t : tuples){
			Tuple n;
//cout << "PROJECTION: USING ORDER: " << (int) order.size() << endl;
//cout << "PROJECTION: USING TUPLE: " << n.size() << endl;
			for(auto index : order){
//cout << "PROJECTION: ADDING TUPLE: " << t[index] << endl;
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
					r += "=";
			/* 	Somehow this line:
					r += scheme.attrs[i] += "='";
				was outputting ridiculous lines where
				the number of =' was equal to the i+1.
				NO IDEA how or why, but separating
				those two fixed it.
				Man, C++ += is a weird beast;
			*/		
					r += t[i];
					if(i < t.size() - 1){
						r += ", ";
					}
				}
				if(scheme.attrs.empty()){
					return "";
				}
				r += "\n";
			}
		}
		return r;
	}

	string toString(Relation & with){
		string r;
//		r += Name += "\n";
		if(!tuples.empty()){
			for (Tuple t : tuples){
				if(count(with.tuples.begin(), with.tuples.end(), t) == 0){ 
					// prints difference of relations by 
					// skipping tuples in "with"
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
					r += t[i];
					r += "' ";
				}
//				if(scheme.attrs.empty()){
//					r += "EMPTY SCHEME";
//				}
				r += "\n";
			}}
		}
		return r;
	}

};

#endif
