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
		out << "\nRule Evaluation\n";
		unsigned passes = 0;
		int preDelta;
		do{
			preDelta = DB.size();
			for(unsigned i = 0; i < DProg.Rules.size(); i++){
				out << DProg.Rules[i].toString() << endl;
				vector<Relation> tails;
				for(unsigned k = 0; k < DProg.Rules[i].tail.size(); k++){
					tails.push_back(DB.predPrep(DProg.Rules[i].tail[k]));
				}
				Relation sum = tails[0];
				for(unsigned d = 1; d < tails.size(); d++){
					sum = sum.join(tails[d]);
				}
				// ok, now project to match head.scheme
				// make column vector
				vector<int> cols;
				for (int z = 0; z < (int) DProg.Rules[i].head.params.size(); z++){
					// if param is var, then add param # to cols
					// somehow avoid doubles
			
					if(DProg.Rules[i].head.params[z].ID)
						cols.push_back(z);
					for (int k = 0; k < z; k++){
						if(DProg.Rules[i].head.params[z].value == DProg.Rules[i].head.params[k].value)
							cols.pop_back();
					}	// this is checking if i've already added that var,
						// then deleting it from end if I have.
						// "slow" but should work well.
				}
				sum = sum.project(cols);
				// then rename to match head.scheme
				vector<string> vars;
				for (int z = 0; z < (int) cols.size(); z++){
					int index = cols[z];
					vars.push_back(DProg.Rules[i].head.params[index].value);
				}
				sum = sum.rename(vars);
				out << sum.toString() << endl;
				// at this point "sum" should be the fully projected and stuff
				// and ready to union with the stuff in the DB
				DB.relations[DProg.Rules[i].head.label].unionWith(sum);

			}
			passes++;
		}while(!((DB.size() - preDelta) == 0));

		out << "Converged after " << passes << " through the Rules." << endl;
		
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
