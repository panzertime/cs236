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
#include "Predicate.h"
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
		Database DB = Database(DProg);
		out << DB.toString();
		// do rule eval here 
		/* algo:
			for each rule in DProg.rules
				for each pred in tail
					queryEval(pred)
				join each of these - maybe just join to preceding, then forget it?
				project to match head.scheme
				rename to match head.scheme
				union with proper relation in DB.relations
		*/
		for(unsigned i = 0; i < DProg.Rules.size(); i++){
			vector<Relation> tails;
			for(unsigned k = 0; k < DProg.Rules[i].tail.size(); k++){
				tails.push_back(DB.predPrep(DProg.Rules[i].tail[k]));
			}
			Relation sum = tails[0];
			for(unsigned d = 1; d < tails.size(); d++){
				sum = sum.join(tails[d]);
			}
		}	

		
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
