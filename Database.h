#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include "Relation.h"
#include "Scheme.h"

class Database {

public:

	Database(){
	}

	virtual ~Database(){
	}

	map<string,Relation> relations;

	void addRelation(string name, Scheme sch){
		//add relation with given name and scheme to 
		//relations.
		//Do this for each Predicate in Schemes
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
