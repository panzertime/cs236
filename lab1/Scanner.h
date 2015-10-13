/*
*
*	Scanner class.
*	Scanners get chars from an infile ()
*	using a private getnext function.  These blocks of chars
*	become tokens thru a tokenizer and then are stored in a list
*	of Tokens.
*	Scanner always knows how many tokens have been read
*	and how many lines have been read.
*
*	(C) RT Hatfield, 2014, 2015
*
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

	char lineComment(char init){
		string words;
		words += '#';

		// what if comment is nothing but #\n?
		// then it's an empty comment.


		// I think the thing to do in that case, is have just an empty comment
		// '#' and return \n back into skipSkippable to deal with
		
		while(true){
			if(init == '\n'){
				Stowage.push_back(Token(words, Total, COMMENT));
				Total++;
				return src.get();
			}
			if(init == EOF){
				Token undef = Token(words, Total, UNDEFINED);
				Stowage.push_back(undef);
				return init;
				// must return EOF so that it can fall all the
				// way back to the EOF token, exit gracefully
			}
			words += init;
			init = src.get();
		}
		return init;
	}
	
	char blockComment(char init){
		unsigned innerLines = 0;
		string words;
		words += '#';
		words += '|';
		while(true){
			if(init == '\n'){
			innerLines++;
			}
			if(init == EOF){
				Token undef = Token(words, Total, UNDEFINED);
				Stowage.push_back(undef);
				Total += innerLines;

				return init;
				// must return EOF in order to get EOF token, exit gracefully
			}
			if(init == '|'){
				words += '|';
				init = src.get();
				if(init == '#'){
					words += init;
					// block comment complete
					Stowage.push_back(Token(words, Total, COMMENT));
					Total += innerLines;
					return src.get();
					// so that we start with new comment
					// instead of recycling current '#'
				}
				words += init;
				}
			else {
				words += init;
							init = src.get();


			}
			
		}
		return init;
	}
	


	char skipSkippable(char init){  
		// skip whitespace
		// handle comments
		// count line numbers
		while(isspace(init) && src.good()){
			if(init == '\n' || init == '\f' || init == '\r'){
				Total++;
			}
			init = src.get();
			//keeps going until character is not w.s.
		}
		if(init == '#'){
			init = src.get();
			// block comment: continue until |#
			if(init == '|'){
				init = blockComment(src.get());	
				// returns newline or EOF
			}
			// line comment: continue to end of line or file
			else {
				init = lineComment(init);
				// returns newline or EOF
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
				
			

	void catchKeywords(string words){
		if(words == "Schemes"){
			Stowage.push_back(Token(words,Total,SCHEMES));
		}
		else if(words == "Facts"){
			Stowage.push_back(Token(words,Total,FACTS));
		}
		else if(words == "Rules"){
			Stowage.push_back(Token(words,Total,RULES));
		}
		else if(words == "Queries"){
			Stowage.push_back(Token(words,Total,QUERIES));
		}
		// not a keyword, create normal ID
		else if(words.size() > 0 && isalpha(words[0])){
			Stowage.push_back(Token(words,Total,ID));
		}
	}

	char scanID(char init){  
		string words;
		// if a non-alnum init makes its way in here,
		// it's because it's already been thru the getNext
		// token loop and been rejected
		if(!isalnum(init)){
			words += init;
			Token undef = Token(words, Total, UNDEFINED);
			Stowage.push_back(undef);
			return src.get();
			// always return the last char scanned!  getNext
			// loop expects that.  but if we return what
			// we were passed, then we end up with a 
			// forever loop.
		}

		while(isalnum(init)){
			words += init;
			init = src.get();
		}
		
		// we now have an ID!
		// filter out keywords and manufacture IDs
		catchKeywords(words);

		return init;
		// always returns the last char scanned, even
		// if "illegal," so that it goes back into the 
		// getNext token loop and gets caught if it's good.
	}	
	
	char scanString(char init){
		string words;
		words += '\'';
		// original design ignored all the apostrophes,
		// now we keep them if properly escaped.
		unsigned innerLines = 0;
		while (true){
			if(init == '\''){
				// check if this is escaped, if so continue
				init = src.get();
				if (init == '\''){
					words += '\'';
					words += '\'';
					init = src.get();
				}
				else {
					words += '\'';
					Stowage.push_back(Token(words,Total,STRING));
					Total += innerLines;
					return init;
				}
			}
			if(init == '\n'){
				// we keep a separate count of how long this 
				// string is because the token's line
				// is expected to be where it starts, not ends
				innerLines++;
				words += init;
				init = src.get();		
			}
			else if(init == EOF){
				// Strings containing EOF are undefined
				// Create UNDEF but allow getNext to 
				// handle EOF and gracefully exit
				Token undef = Token(words, Total, UNDEFINED);
				Stowage.push_back(undef);
				Total += innerLines;
				return EOF;
			}
			else {
				words += init;
				init = src.get();
			}

		}
		return init;
	}

	void getNext(){
		// read some chars, put some Tokens
		// (top level of parsing mechanics)
		// returns void since we no longer care
		// where we left off, as we now ALWAYS
		// scan all input
		char c = src.get();
	
		/*
		*  weirdly, .get() keeps going past EOF
		*  thus it is wonderful that I happened
		*  upon ios.good()
		*
		*   yet another reason C++ is an abomination
		*/
		
		while(src.good()) {
			string words;
			c = skipSkippable(c);
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
					break;
				case '\'' :
					c = src.get();
					c = scanString(c);
					break;
				case EOF :
					Stowage.push_back(Token("", Total, DUPE_EOF));
					return;
				case '*' :
					Stowage.push_back(Token("*", Total, MULTIPLY));
					c = src.get();
				case '+' :
					Stowage.push_back(Token("+", Total, ADD));
					c = src.get();
				default :
					c = scanID(c);
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
