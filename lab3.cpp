/*	
*
*	Interpreter for Datalog language - part three of
*	Datalog REPL
*
*/

#include "Token.h"
#include "Scanner.h"
#include "DatalogProgram.h"
#include "Parser.h"
#include "Database.h"
#include <vector>
#include <fstream>

int main(int argc, const char** argv){
	
	ifstream in;
	in.open(argv[1]);
	ofstream out;
	out.open(argv[2]);

	Scanner s(in);
	s.scan();
	vector<Token> v = s.getTokens();
	Parser p(v);
	try{
		DatalogProgram DProg = p.parse();
		DProg.createDomain();
	//	out << "Success!" << endl;
	//	out << DProg.toString();
		Database DB = Database(DProg);
		out << DB.toString();
		out << "\nQuery Evaluation\n\n";
		for (auto p : DProg.Queries){
			out << p.toString() << "? ";
			out << DB.queryEval(p);
			
		}
	}
	catch (Token e){
		out << "Failure!" << endl;
		out << "  " ;
		e.print(out);
		
	}
/*	if (err)
		out << "Input Error on line " << err << endl;
	in.close();
	if (!err)
		out << "Total Tokens = " << v.size(); */
	in.close();
	out.close();
}
