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

#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Scanner {

	unsigned Total;
	vector<Token> Stowage;
	ifstream & src;
	
	void skipSkippable(char init){
		//skip whitespace
		//skip comments
		//count linenumbers
cout << "skipping" << endl;
		while(isspace(init)){
cout << "w.s.!" << endl;
			if(init == '\n'){
				Total++;
			}
			init = src.get();
			//keeps going until character is not w.s.
		}
		if(init == '#'){
cout << "comment" << endl;
			//continue to end of line
			while(init != '\n'){
				init = src.get();
			}
			skipSkippable(init);
			//to get all comments in a row
			//as well as the newline
			//test by doing multiline comments
		}
	}		

	void getNext(){
		// read some chars, return Token
	
		char c;
		// weirdly, .get() keeps going past EOF
		while((c = src.get()) && src.good()){
cout << "before skip: " << c << endl;
			skipSkippable(c);
cout << "after skip: " << c << endl;
			switch(c){
				case ',' :
cout << c << endl;
					Stowage.push_back(Token(",",Total,COMMA));
					break;
				case '.' :
					//period
					break;
				case '?' :
					//
					break;
				case '(' :
					
					break;
				case ')' :
					
					break;
				case ':' :
					//also find :-
					break;
				case '\'' :
					//scan whole string
					break;
				default :
					//scanID
					break;
			}
		}		
	}

public:
	Scanner(ifstream & in):src(in){
		Total = 1;
		//because the first line is called "line one"
		//
		//	duh
	}
	
	virtual ~Scanner(){
	}

	void scan(){
		//repeatedly do:
		//	getNext()
		//	tokenize();
		//	new token, add to Stowage
		getNext();
	}

	vector<Token> getTokens(){
		return Stowage;
	}

};
	

#endif
