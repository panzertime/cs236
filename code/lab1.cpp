/*	
*
*	Scanner for Datalog language - part one of
*	Datalog Interpreter
*
*	(C) RT Hatfield, 2014, 2015
*
*/

#include "Token.h"
#include "Scanner.h"
#include <vector>
#include <fstream>

int main(int argc, const char** argv){
	
	ifstream inFile;
	inFile.open(argv[1]);

	Scanner source(inFile);
	source.scan();
	vector<Token> tokenVector = source.getTokens();

	for(auto token : tokenVector){
		token.print(cout);
	}

	cout << "Total Tokens = " << tokenVector.size();
	
	inFile.close();

}
