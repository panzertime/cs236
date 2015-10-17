/*	
*
*	Scanner for Datalog language - part one of
*	Datalog REPL
*
*/

#include "Token.h"
#include "Scanner.h"
#include "DatalogProgram.h"
#include "Parser.h"
#include <vector>
#include <fstream>

int main(int argc, const char** argv){
	
	ifstream in;
	in.open(argv[1]);


	Scanner s(in);
	s.scan();
	vector<Token> v = s.getTokens();
	Parser p(v);
	try{
		DatalogProgram DProg = p.parse();
		DProg.createDomain();
		cout << "Success!" << endl;
		cout << DProg.toString();
		
	}
	catch (Token e){
		cout << "Failure!" << endl;
		cout << "  " ;
		e.print(cout);
		
	}
/*	if (err)
		out << "Input Error on line " << err << endl;
	in.close();
	if (!err)
		out << "Total Tokens = " << v.size(); */
	in.close();
}

