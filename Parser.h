#ifndef PARSER_OBJECT_H
#define PARSER_OBJECT_H

#include "Token.h"
#include "Kind.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class Parser {

private:
	vector<Token> tokens;
	int at;
	DatalogProgram DP;
	Predicate currPred;
	Predicate noP;
	Rule currentRule;
	Rule noR;

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

	DatalogProgram parse(){
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
		currPred = noP;
		// erase currPred ?
		// in fact... this is a problem.  we need to make
		// a *new* predicate every time.  maybe return 
		// a predicate from parsePredicate() ?
	}

	void parsePredicate(){
		match(ID);
		currPred.label=tokens[at - 1].Value;
		match(LEFT_PAREN);
		parseParamList();
		match(RIGHT_PAREN);
	}

	void parseParamList(){
		/* check for either just the param, or more lists
		 * then */

		parseParam();
		if(tokens[at].tokenType == COMMA){
			match(COMMA);
			parseParamList();
		}
		//might need extra increment here, idk
	}

	void parseParam(){
		//either string or ID
		if (tokens[at].tokenType == ID){
			match(ID);
			p.name = tokens[at-1].Value;
		}
		else {
			match(STRING);
			p.value = tokens[at-1].Value;
		}
		currPred.addParam();
	}

	void parseFactList(){
		if(tokens[at].tokenType != RULES){
			parseFact();
			parseFactList();
		}
	}

	void parseFact(){
		Predicate p = parsePredicate();
		match(PERIOD);
		DP.addFact();
		// same issue as parseScheme()
	}

	void parseRuleList(){
		if(tokens[at].tokenType != COMMA){
			parseRule();
			parseRuleList();
		}
	}

	void parseRule() {
		parsePredicate();
		currRule.addHead(currPred);
		currPred = noP;
		match(COLON_DASH);
		parsePredicateList();
		DP.addRule(currRule);
		currRule = noR;
		
		//
		// wow, how do i know where / when to put these predicates!
		// honestly why not just assume the first pred in any rule
		// is before a :-, this is not complicated
	}

	void parsePredList(){
		parsePredicate();
		currRule.addTail(currPred);
		currPred = noP;
		if(tokens[at].tokenType == COMMA){
			match(COMMA);
			parsePredList();
		}
		else {
			match(PERIOD);
		}
	}

	void parseQueryList() {
		parseQuery();
		if(at > tokens.size()){
			return;
			//we should be done, right?
		}
		else{
			parseQueryList();
		}
	}

	void parseQuery() {
		parsePredicate();
		DP.addQuery(currPred);
		currPred = noP;
		match(Q_MARK);
	}



		






}; //close class definition

#endif
