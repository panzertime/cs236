#ifndef SCHEME_H
#define SCHEME_H

class Scheme {

public:
	Scheme(){
		//need to be able to build a Scheme
		//from a relation: whether to do this
		//in the constructor or in main,
		//unsure.  Probably not important.
	}

	virtual ~Scheme(){
	}

	vector<string> attrs;

};

#endif
