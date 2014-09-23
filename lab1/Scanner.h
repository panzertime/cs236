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
	
	char skipSkippable(char init){
		//skip whitespace
		//skip comments
		//count linenumbers
		while(isspace(init)){
			if(init == '\n'){
				Total++;
			}
			init = src.get();
			//keeps going until character is not w.s.
		}
		if(init == '#'){
			//continue to end of line
			while(init != '\n'){
				init = src.get();								
			}
			init = skipSkippable(init);
			//to get all comments in a row
			//as well as the newline
			//test by doing multiline comments
			//
			//this actually raised a huge issue.
			//getNext knew where the caret was,
			//but not the last char read.  
			//introducing a return almost fixed
			//it, but i forgot to carry thru
			//recursion
		}
	return init;
	}		

	void getNext(){
		// read some chars, put some Tokens
	
		char c;
		// weirdly, .get() keeps going past EOF
		// thus it is wonderful that I happened
		// upon ios.good()
		while((c = src.get()) && src.good()){
			string words;
			c = skipSkippable(c);
			switch(c){
				case ',' :
					Stowage.push_back(Token(",",Total,COMMA));
					break;
				case '.' :
					Stowage.push_back(Token(".",Total,PERIOD));
					break;
				case '?' :
					Stowage.push_back(Token("?",Total,Q_MARK));
					break;
				case '(' :
					Stowage.push_back(Token("(",Total,LEFT_PAREN));
					break;
				case ')' :
					Stowage.push_back(Token(")",Total,RIGHT_PAREN));
					break;
				case ':' :
					c = src.get();
					if (isspace(c)){
						Stowage.push_back(Token(":",Total,COLON));
					}
					else {
						Stowage.push_back(Token(":-",Total,COLON_DASH));
					}
					break;
				case '\'' :
					c = src.get();
					do {
						words += c;
						c = src.get();
					} while (c != '\'');
					Stowage.push_back(Token(words,Total,STRING));
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
		getNext();
	}

	vector<Token> getTokens(){
		return Stowage;
	}

};
	

#endif
