#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include "basicAutomata.h"
#include "otherAutomata.h"
#include "database.h"
//#include "parseClass.h"


const int TEST_NUM = 20;
const string TESTS[] = {
		"./tests/join1.txt", "./tests/join2.txt", "./tests/join3.txt", "./tests/join4.txt", "./tests/join5.txt", "./tests/join7.txt", "./tests/join6.txt", "./tests/relate1.txt",
		"./tests/relate2.txt", "./tests/relate3.txt", "./tests/relate4.txt", "./tests/relate5.txt","./tests/relate6.txt", "./tests/relate7.txt",
		"./tests/relate8.txt", "./tests/relate9.txt", "./tests/relate10.txt", "./tests/relate11.txt", "./tests/relate12.txt", "./tests/relate0.txt"
};


//----------------------------------------------
// Clean Up
//----------------------------------------------
void detoxAutomata(vector<BaseAutomata*> &myAutomata) {
    for (int i = 0; i < myAutomata.size(); i++) {
        delete myAutomata.at(i);
    }
    //    cout << "okay?\n";
}
//----------------------------------------------

//
// Piece of Lexical Analyzer
//
void initiateAutomata(vector<BaseAutomata*> &myAutomata) {
	myAutomata.push_back(new comma());
	myAutomata.push_back(new period());
	myAutomata.push_back(new Q_mark());
	myAutomata.push_back(new left_paren());
	myAutomata.push_back(new right_paren());
	myAutomata.push_back(new colon());
	myAutomata.push_back(new multiplyClass());
	myAutomata.push_back(new addClass());
	myAutomata.push_back(new schemes());
	myAutomata.push_back(new facts());
	myAutomata.push_back(new colon_dash());
	myAutomata.push_back(new queries());
	myAutomata.push_back(new rules());
	myAutomata.push_back(new stringClass());
	myAutomata.push_back(new BlockComment());
	myAutomata.push_back(new Comment());
	myAutomata.push_back(new IDautomata());
	myAutomata.push_back(new Undefined());
	myAutomata.push_back(new FileEnd());
}

//
// Piece of Lexical Analyzer
//
void getLongest(vector<BaseAutomata*> &myAutomata, int &longest, int & lengthIndex, reader &myReader) {
    for (int i = 0; i < myAutomata.size(); i++) {
        int length = myAutomata[i]->read(myReader);
        if ( length > longest) {
            longest = length;
            lengthIndex = i;
        }
    }
}

//
// Piece of Lexical Analyzer
//
void addToken(reader &myReader, vector<BaseAutomata*> &myAutomata, queue<token> &myTokens, vector<string> &failStrings, queue<string> &tokenStrings, int & lengthIndex, int &longest, int & line_num) {
    //remove longest amount of chars and output token
    parse myParser;
    token t = myAutomata[lengthIndex]->tokenOut();
    if (t != COMMENT && t != UNDEFINED) {
        stringstream ss;
        myTokens.push(t);
        string s = myReader.deleteChar(longest);
        if (t == STRING || t == ID || t == ADD || t == MULTIPLY) {
          //cout << tokenNumCheck++;
          tokenStrings.push(s);
        }
        ss << "(" << myParser.toString(t) << ",\"" << s << "\"," << line_num << ")\n";
        failStrings.push_back(ss.str());
//                cout << ss.str();
    }else {
        myReader.deleteChar(longest);
    }
}

//----------------------------------------------
// Part 1: Lexical Analyzer
//----------------------------------------------
int getTokens(reader &myReader, vector<BaseAutomata*> &myAutomata, queue<token> &myTokens, vector<string> &failStrings, queue<string> &tokenStrings) {
    int token_num = 0;
    int line_num = 0;

    //int tokenNumCheck = 0;
    while (!myReader.isEmpty()) {
        int longest = 0;
        int long_index = 0;
        line_num = myReader.curLineOut();
//        cout << "searching for longest\n";
        getLongest(myAutomata, longest, long_index, myReader);
//        cout << "found longest\n";
        if (longest == 0) {
            //remove 1 char off the top
            myReader.deleteChar(1);
        }else {
            token_num++;
            addToken(myReader, myAutomata, myTokens, failStrings, tokenStrings, long_index, longest, line_num);
        }
//        cout << line_num << endl;
    }
  //cout << endl;
    token_num++;
    stringstream ss;
    myTokens.push(MEOF);
    ss << "(EOF,\"\"," << myReader.curLineOut() - 1 << ")\n";
//    ss1 << "Total Tokens = " << token_num << endl;
    failStrings.push_back(ss.str());
//    output.push(ss1.str());
//    cout << "lexer finished\n";
    return token_num;
}
//----------------------------------------------


//----------------------------------------------
// Prep for Part 3
//
class datalogClass* makeDatalog(class parse* parser) {
    class datalogClass* d = parser->datalogOut();
//    cout << d->toString();
    return d;
}

//----------------------------------------------
// Part 2: Parsing
//----------------------------------------------
class parse* parseStart(queue<token> &myTokens, vector<string> &failStrings, queue<string> &tokenStrings) {
	int size = myTokens.size();
    class parse* myParser = new class parse();
//    cout << myStrings.size() << endl;
    int result = myParser->datalogParser(myTokens, tokenStrings);
    if (result == myTokens.size()) {
//        cout << "Success!\n";
    }else {
        cout << "Failure!\n  " << failStrings.at(size - myTokens.size());
        return NULL;
    }
    return myParser;
}
//----------------------------------------------

//----------------------------------------------
// Part 3:  Relations
//----------------------------------------------
class Database* createDatabase(class datalogClass* datalog) {
    return new class Database(datalog);
}
//----------------------------------------------

void program( string file ) {
	reader myReader;
	bool check = myReader.readIn( file );
	if (!check) {
	  return;
	}
	vector<BaseAutomata*> myAutomata;
	initiateAutomata(myAutomata);

	vector<string> failStrings;
	queue<string> tokenStrings;
    queue<token> myTokens;
    getTokens(myReader, myAutomata, myTokens, failStrings, tokenStrings);
    
    class parse* parser = parseStart(myTokens, failStrings, tokenStrings);
    
    class datalogClass* datalog;
    if (parser != NULL) {
      datalog = makeDatalog(parser);
    }
    
    class Database* database;
    if (parser != NULL) {
        database = createDatabase(datalog);
        string s = database->interpretStart();
        if (s != "") {
//        	cout << s;
//        	cout << "Success!\n";
        	ofstream outFile;
        	outFile.open( "testOutput" );
        	outFile << s;
        	outFile.close();
        }
    }
    
    delete database;
    delete datalog;
    delete parser;
    detoxAutomata(myAutomata);
}


// Main Program
int main(int argc, char *argv[]) {
    if (argc != 2) {
//        cout << "Not correct argument, end\n";
    	cout << "Automatic Testing:\n";
    	for (int i = 0; i < TEST_NUM; i++) {
    		cout << "\nTest number " << i + 1 << "\n";
    		program( TESTS[i] );
    		cout << "Success!\n";
    	}
    }else {
    	program( argv[1] );
    }



//	for (int i = 0; i < TEST_NUM; i++) {
//		cout << "\nTest number " << i + 1 << "\n";
//		program( TESTS[i] );
//	}

	return 0;
}
