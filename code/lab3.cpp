/*	
*
*	Interpreter for Datalog language - part three of
*	Datalog
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
	//	cout << DB.toString();
	//	cout << "\nQuery Evaluation\n\n";
		for (auto p : DProg.Queries){
			cout << p.toString() << "? ";
			cout << DB.queryEval(p);
			
		}
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
