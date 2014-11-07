#ifndef SCHEME_H
#define SCHEME_H

#include <vector>
#include "Predicate.h"
#include "Parameter.h"

class Scheme {

public:
	Scheme(Predicate & src){
		//need to be able to build a Scheme
		//from a relation: whether to do this
		//in the constructor or in main,
		//unsure.  Probably not important.
		//
		//Why a relation?  Make it from a predicate
		//that you pull from your DP.
		
		for (Parameter p : src.params){
			attrs.push_back(p.value);
			if(p.ID){
				vars.push_back(1);
			}
			else {
				vars.push_back(0);
			}
		}	
	}

	Scheme(vector<string> & src){
		attrs = src;
		for (auto a : src){
			vars.push_back(1);
		}
	}

	Scheme(){
		// nothing
	}

	virtual ~Scheme(){
	}

	vector<string> attrs;
	vector<bool> vars;

};

#endif
