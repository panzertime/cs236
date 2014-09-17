/*	
*
*	Scanner for Datalog language - part one of
*	Datalog REPL
*
*/

#include "Token.h"
#include <fstream>

int main(int argc, const char** argv){

	Token t = Token("Dicks in Pussies",3,ID);
	ofstream out;
	out.open(argv[1]);
	t.print(out);
	out.close();
}
