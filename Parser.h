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

	DatalogProgram parse();
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
	return DP;
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
		// in fact... this is a problem.  we need to make
		// a *new* predicate every time.  maybe return 
		// a predicate from parsePredicate() ?
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

	void parseFactList(){
		if(tokens[at].tokenType != RULES){
			parseFact();
			parseFactList();
		}
	}

	void parseFact();
		parsePredicate();
		DP.addFact(currPred);
		// same issue as parseScheme()
	}

	void parseRuleList();
		if(tokens[at].tokenType != QUERIES){
			parseRule();
			parseRuleList();
		}
	}

	void parseRule() {
		// let's fix the predicate issue first, same issue here
	}



		






}; //close class definition

#endif
