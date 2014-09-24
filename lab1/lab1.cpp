/*	
*
*	Scanner for Datalog language - part one of
*	Datalog REPL
*
*/

#include "Token.h"
#include "Scanner.h"
#include <vector>
#include <fstream>

int main(int argc, const char** argv){
	
	ifstream in;
	in.open(argv[1]);
	ofstream out;
	out.open(argv[2]);

	Scanner s(in);
	unsigned err = s.scan();
	vector<Token> v = s.getTokens();
	for(auto t : v){
		t.print(out);
	}
	if (err)
		out << "Input Error on line " << err << endl;
	in.close();
	if (!err)
		out << "Total Tokens = " << v.size();
	out.close();
}
