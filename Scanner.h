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
		while(isspace(init) && src.good()){
			if(init == '\n' || init == '\f' || init == '\r'){
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
	
	char scanID(char init){
		string words;
		while(isalnum(init) ){
			words += init;
			init = src.get();
			//possible problem source...
			//the order of the .get() can be an issue

		}
//cout << "adding ID at: " << Total << endl;
		//we now have an ID!
		if(words == "Schemes"){
			Stowage.push_back(Token(words,Total,SCHEMES));
			return init;
		}
		else if(words == "Facts"){
			Stowage.push_back(Token(words,Total,FACTS));
			return init;
		}
		else if(words == "Rules"){
			Stowage.push_back(Token(words,Total,RULES));
			return init;
		}
		else if(words == "Queries"){
			Stowage.push_back(Token(words,Total,QUERIES));
			return init;
		}
		else if(words.size() > 0 && isalpha(words[0])){
			Stowage.push_back(Token(words,Total,ID));
			return init;
		}

		throw Total;

	}	
	
	char scanString(char init){
		string words;
		while (init != '\''){
			words += init;
			init = src.get();
			if(init == '\n' || init == EOF){
				throw Total;

			}
		}// while (init != '\'');
		Stowage.push_back(Token(words,Total,STRING));
		// no extra src.get() because that do-while does it for us
		// otherwise we end up doing two strings :P
		return src.get();
	}

	void getNext(){
		// read some chars, put some Tokens
	
		char c = src.get();
		// weirdly, .get() keeps going past EOF
		// thus it is wonderful that I happened
		// upon ios.good()
		while(src.good()) {
			string words;
//cout << "Line: " << Total << endl;
//cout << "char: " << c << endl;
			c = skipSkippable(c);
//cout << "skipped to: " << c << endl;
			switch(c){
				case ',' :
					Stowage.push_back(Token(",",Total,COMMA));
					c = src.get();
					break;
				case '.' :
					Stowage.push_back(Token(".",Total,PERIOD));
					c = src.get();
					break;
				case '?' :
					Stowage.push_back(Token("?",Total,Q_MARK));
					c = src.get();
					break;
				case '(' :
					Stowage.push_back(Token("(",Total,LEFT_PAREN));
					c = src.get();
					break;
				case ')' :
					Stowage.push_back(Token(")",Total,RIGHT_PAREN));
					c = src.get();
					break;
				case ':' :
					c = src.get();
					if (c != '-'){
						Stowage.push_back(Token(":",Total,COLON));
					}
					else if (c == '-'){
						Stowage.push_back(Token(":-",Total,COLON_DASH));
						c = src.get();
					}
					//c = src.get(); // have to do twice so we don't iterate again with -
					break;
				case '\'' :
					c = src.get();
					c = scanString(c);
					break;
				case EOF :
					return;
				
				default :
//cout << "scanning: " << c << " line " << Total << endl;
					c = scanID(c);
					break;
			}

		} // while (src.good());		
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
