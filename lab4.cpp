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
#include "Parameter.h"
#include <string>
#include <vector>
#include <fstream>

vector<int> numbers(vector<Parameter> & parameters, vector<string> & attrs){
//				for (int z = 0; z < (int) DProg.Rules[i].head.params.size(); z++){
//					// if param is var, then add param # to cols
//					// somehow avoid doubles
//			
//					if(DProg.Rules[i].head.params[z].ID)
//						cols.push_back(z);
//					for (int k = 0; k < z; k++){
//						if(DProg.Rules[i].head.params[z].value == DProg.Rules[i].head.params[k].value)
//							cols.pop_back();
//					}	// this is checking if i've already added that var,
//						// then deleting it from end if I have.
//						// "slow" but should work well.
//				}
				// redo column generation
				// take next attr in head, find column in joined, then push_back
	vector<int> cols;
	for (int z = 0; z < (int) parameters.size(); z++){

		for (int k = 0; k < (int) attrs.size(); k++){
			if (parameters[z].value == attrs[k]){
				cols.push_back(k);
			}
		}
	}
	
	return cols;
}

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
		out << "Rule Evaluation\n\n";
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
//	cout << "Joined vector: " << endl;
//	cout << sum.toString();
//	cout << "	but: " << endl;
//	for(set<Tuple>::iterator roy = sum.tuples.begin(); roy != sum.tuples.end(); roy++){
//		for( auto ray : *roy){
//	cout << ray << " - ";
//	}
//	cout << endl;
//	}
				vector<int> cols = numbers(DProg.Rules[i].head.params, sum.scheme.attrs);
					
//	cout << "Columns: " << endl;
//	for (auto things : cols){
//	cout << things << endl;
//	}
				sum = sum.project(cols);
//	cout << "Projected vector: " << endl;
//	cout << sum.toString();
				// then rename to match scheme in DB already
				// do like DProg.Rules[i].head.label
				vector<string> vars;
				for (int z = 0; z < (int) DB.relations[DProg.Rules[i].head.label].scheme.attrs.size(); z++){ 
					vars.push_back(DB.relations[DProg.Rules[i].head.label].scheme.attrs[z]);
				}
				sum = sum.rename(vars);
				// at this point "sum" should be the fully projected and stuff
				// and ready to union with the stuff in the DB
				out << sum.toString(DB.relations[DProg.Rules[i].head.label]);
				DB.relations[DProg.Rules[i].head.label].unionWith(sum);
				//and now print... but how?
				//check each tuple?  probably all we can do.

			}
			passes++;
		}while(!((DB.size() - preDelta) == 0));
		
		out << endl;
		out << "Converged after " << passes << " passes through the Rules." << endl;
		// funny note: i kept forgetting to add "passes" even though i noticed it several times
		// because the variable is also called passes.  this isn't the first time
		// i've had this problem.  tell john carmack idk
		
		out << endl;
		out << DB.printRelations();


		
		out << "Query Evaluation\n\n";
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


