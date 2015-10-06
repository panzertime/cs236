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
	
	bool error;
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
				Stowage.push_back(Token("", 
						Total, 
						DUPE_EOF));
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
		words += init;
		while(true){
			init = src.get();
			if(init == '\n'){
			innerLines++;
			}
			if(init == EOF){
				Token undef = Token(words, Total, UNDEFINED);
				Stowage.push_back(undef);
				Stowage.push_back(Token("", 
							Total + innerLines - 1, 
							DUPE_EOF));
				error = 0;
				return init;
				// must return EOF in order to get EOF token, exit gracefully
			}
			if(init == '|'){
				words += '|';
				init = src.get();
				if(init == '#'){
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
			}
		}
		return init;
	}
	


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
			string words;
			init = src.get();
			// block comment: continue until |#
			if(init == '|'){
				init = blockComment(src.get());	
				// returns newline or EOF
			}
			//continue to end of line or file
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
		else if(words.size() > 0 && isalpha(words[0])){
			Stowage.push_back(Token(words,Total,ID));
		}
	}

	char scanID(char init){  
		string words;
		// if a non-alnum init makes its way in here,
		// it's because it's already been thru the main
		// token loop and been rejected
		if(!isalnum(init)){
			words += init;
			Token undef = Token(words, Total,UNDEFINED);
			Stowage.push_back(undef);
			return src.get();
			// always return the last char scanned!  main
			// loop expects that.  but if we return what
			// we were passed, then we end up with a 
			// forever loop.
		}


		// problem I'm running into here is that
		// functionality is bleeding between scanID() and getNext().
		// I need to review where exactly init is supposed to
		// be at each stage
		

		// aha!  what's happening is that i'm going all the way to the
		// space, whereas originally I just took in alnums,
		// therefore the next char would be a : rather than a space and 
		// the last one being :, making the string Queries: and also 
		// triggering an UNDEF.
		// So I should go to the first non-alnum (spaces are non-alnum),
		// and if any ID starts with a non-alnum then I make
		// an UNDEF.  in the inner block if I hit a non-alnum, blah
		// blah

		while(isalnum(init)){
			words += init;
			init = src.get();
			//possible problem source...
			//the order of the .get() can be an issue
		}
//cout << "adding ID at: " << Total << endl;
		//we now have an ID!
		
		catchKeywords(words);

		return init;
		// always returns the last char scanned, even
		// if "illegal," so that it goes back into the 
		// main token loop and gets caught if it's good.
	}	
	
	char scanString(char init){
		string words;
		words += '\'';
		// original design ignored all the apostrophes,
		// now we keep them.
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
				// Create both UNDEF and EOF tokens
				Token undef = Token(words, Total, UNDEFINED);
				Stowage.push_back(undef);
				Stowage.push_back(Token("", Total + innerLines - 1, DUPE_EOF));
				error = 0;
				return init;
			}
			else {
				words += init;
				init = src.get();
			}

		}
		// no extra src.get() because that do-while does it for us
		// otherwise we end up doing two strings :P
		return init;
	}

	void getNext(){
		// read some chars, put some Tokens
		// (top level of parsing mechanics)
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
					Stowage.push_back(Token("",(Total - 1),DUPE_EOF));
					return;
				
				default :
//cout << "scanning: " << c << " line " << Total << endl;
					c = scanID(c);
					break;
			}
			if(error){
				error = 0;
			//	return;
				// do not need to return anymore, apparently
			}
		} // while (src.good());		
	}

public:
	Scanner(ifstream & in):src(in){
		Total = 1;
		error = 0;
		//because the first line is called "line one"
		//
		//	duh
	}
	
	virtual ~Scanner(){
	}

	unsigned scan(){
		getNext();
		if(error){
			return Total;}
		else
			return 0;
	}

	vector<Token> getTokens(){
		return Stowage;
	}

};
	

#endif
