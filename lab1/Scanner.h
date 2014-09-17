/*
*
*	Scanner class.
*	Scanners get chars from an infile ()
*	using a private getnext function.  These blocks of chars
*	become tokens thru a tokenizer and then are stored in a list
*	of Tokens.
*	Scanner always knows how many tokens have been read.
*
*/

#include "Token.h"
#include <list>
#include <string>
#include <ifstream>

using namespace std;

class Scanner {

	unsigned Total;
	list<Token> Stowage;
	
	void skipSkippable(ifstream & src){
		//skip whitespace
		//skip comments
		//count linenumbers
		char c = src.get();
		while(c.isspace()){
			if(c == '/n'){
				Total++;
			}
			c = src.get();
			//keeps going until c is not w.s.
		}
		if(c == '#'){
			Total++;
			//continue to end of line
		}
	}

	string getNext(ifstream & src){
		// read some chars, return string
		
		char c;
		while(c = src.get()){
			while(c.isspace()){
				if(c == '/n'){
					Total++;
				}
				c = src.get();
	//continually until no more w.s.
			}
			if(c == '#'){
				//Total++;
	//Check here if total count is bad
	//test by adding comments
				while(c != '/n'){
				c = src.get();

		
	}

public:
	Scanner(){
		Total = 0;
	}
	
	virtual ~Scanner(){
	}

	scan(ifstream & in){
		//repeatedly do:
		//	getNext()
		//	tokenize();
		//	new token, add to Stowage
	}

	list<Token> getTokens(){
		return Stowage;
	}

};
	
