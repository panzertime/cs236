#ifndef PARSER_OBJECT_H
#define PARSER_OBJECT_H

//#include "Token.h"
//#include "DatalogProgram.h"
//#include "Predicate.h"
//#include "Rule.h"
//#include "Parameter.h"
#include "Expression.h"
//#include "Parameter.h"

using namespace std;

class Parser {

private:
	vector<Token> tokens;
	unsigned at;
	DatalogProgram DP;
	Predicate currPred;
	Predicate noP;
	Rule currRule;
	Rule noR;

public:
	Parser(vector<Token> & scanned):tokens(scanned){
		at = 0;
	}

	virtual ~Parser(){
	}

	void match(Kind k){
		if(tokens[at].tokenType == k) {
			at++;
		}
		else {
			throw tokens[at];
		}
		// makes sure it's the right kind of terminal,
		// if not throws the offending token (handle
		// it in main, I guess)
	}

	void vacuum(vector<Token>& tokenList){
		for(int i = 0; i < tokenList.size(); i++){
			Token item = tokens[i];
			if(item.tokenType == COMMENT ||
					item.tokenType == UNDEFINED ||
					item.tokenType == DUPE_EOF){
				tokenList.erase(tokenList.begin() + i);
			}
		}
	}


	DatalogProgram parse(){
		vacuum(tokens);
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

		currPred.addParam(parseParam());
		if(tokens[at].tokenType == COMMA){
			match(COMMA);
			parseParamList();
		}
		//might need extra increment here, idk
	}

	Parameter parseParam(){
		// either string or ID
		// nix that: added an Expression type, 
		// LEFT_PAREN param oper param RIGHT_PAREN
		// oper being ADD | MULTIPLY
		Parameter p;
		if (tokens[at].tokenType == ID){
			match(ID);
			p.addName(tokens[at-1].Value);
		}
		else if (tokens[at].tokenType == LEFT_PAREN){
			Expression ex;
			Kind operation;
			match(LEFT_PAREN);
			Parameter left = parseParam();
			if(tokens[at].tokenType == ADD){
				match(ADD);
				operation = ADD;
			}
			else {
				match(MULTIPLY);
				operation = MULTIPLY;
			}			
			Parameter right = parseParam();
			match(RIGHT_PAREN);
			ex.addExpr(left, operation, right);
			return ex;
		}
		else {
			match(STRING);
			p.addValue(tokens[at-1].Value);
		}
		return p;
	}

	void parseFactList(){
		if(tokens[at].tokenType != RULES){
			parseFact();
			parseFactList();
		}
	}

	void parseFact(){
		parsePredicate();
		match(PERIOD);
		DP.addFact(currPred);
		currPred = noP;
		// same issue as parseScheme()
	}

	void parseRuleList(){
		if(tokens[at].tokenType != QUERIES){
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

		// match(PERIOD);
		// because i match it in PredList.
	}

	void parsePredicateList(){
		parsePredicate();
		currRule.addTail(currPred);
		currPred = noP;
//cout << "finna match: " << tokens[at].tokenType << endl;
		if(tokens[at].tokenType == COMMA){
			match(COMMA);
			parsePredicateList();
		}
		else {
			match(PERIOD);
		}
	}

	void parseQueryList() {
		parseQuery();
		if(at > ( tokens.size() - 1 )){
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
