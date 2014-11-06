#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include "relation.h"


class Database {

public:

	Database(){
	}

	virtual ~Database(){
	}

	map<string,Relation> relations;
	

private:


};


#endif 
