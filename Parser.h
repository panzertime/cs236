#ifndef PARSER_OBJECT_H
#define PARSER_OBJECT_H

#include 'Token.h'
#include 'Kind.h'
#include 'DatalogProgram.h'
#include 'Predicate'

using namespace std;

class Parser {

private:
	vector<Token> tokens;
	int at;
	DatalogProgram DP;
	Predicate currPred;

public:
	Parser(vector<Token> & scanned):tokens(scanned){
		at = 0;
	}

	virtual ~Parser(){
	}

	void match(Kind k){
		if(tokens[at].tokenType == k)
			at++;
		else
			throw tokens[at]
		// makes sure it's the right kind of terminal,
		// if not throws the offending token (handle
		// it in main, I guess)
	}

	void parse(){
		match(SCHEMES);
		match(COLON);
		parseSchemeList();
		match(FACTS);
		match(COLON);
		parseFactList();
		match(RULES);
		match(COLON);
		parseRuleList();
		match(QUERIES);
		match(COLON);
		parseQueryList();
	}

private:  	// no idea if this works, we'll see i guess

	void parseSchemeList(){
		parseScheme();
		if(tokens[at].tokenType != FACTS)
			parseSchemeList();
	}

	void parseScheme(){
		parsePredicate();
		DP.addScheme(currPred);
		// erase currPred ?
	}

	void parsePredicate(){
		match(ID);
		currPred.label = tokens[at - 1].Value;
		match(LEFT_PAREN);
		parseParamList();
		match(RIGHT_PAREN);
	}

	void parseParamList(){
		/* check for either just the param, or more lists
		 * then */

		parseParam();
		if(tokens[at].tokenType == COMMA)
			parseParamList();
		//might need extra increment here, idk
	}

	void parseParam(){
		//either string or ID
		if (tokens[at].tokenType == ID){
			match(ID);
			Parameter p();
			p.name = tokens[at-1].Value;
		}
		else {
			match(STRING);
			Parameter p();
			p.value = tokens[at-1].Value;
		}
		currPred.addParam(p);
	}






}; //close class definition

#endif
