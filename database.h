//
//  database.h
//  parser
//
//  Created by Iain Lee on 10/23/16.
//  Copyright © 2016 Iain. All rights reserved.
//

#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "datalogProgram.h"
#include "relationClass.h"
#include "graphClass.h"

using namespace std;

#ifndef database_h
#define database_h

struct myNode {
    string name;
    int index;
};

class Database {

private:
	class datalogClass* datalog;
    vector<class relation*> relations;
    string renameOutput;
    
    void makeRelations();
    void makeTuples();
    vector<string> makeTuple(vector<class ParameterClass*>* params);
    
    // interpretation functions:
    relation* queryFind(relation* relat, PredicateClass* query);
    void projector(relation* &r);
    void renamer(relation* &rel);

    vector<string> projectNames;
    vector<int> projectIndex;
    int relationIndex(string name);
    relation* copyRelation(relation* a);
    int repeatVar(string var);
    
    // Lab 4 functions:
    int convertRules( const vector<RuleClass*> &rules );
    relation* ruler( RuleClass* rule );
    string relationsOut();
    void renameOutputFunc(relation* rel, int b);
    int facts();
    void clear();
    void sort();

//    lab 5 functions:
    graphClass* graph;
    void makeGraph( const vector<RuleClass*>* rules );
    void optimizedRules();
    string graphOut( vector< vector<int> > gr );

public:
    Database(datalogClass* data) : datalog(data) {
        makeRelations();
        sort();
        makeGraph( datalog->rulesOut() );
        cout << "Rule Evaluation\n";
        optimizedRules();
//        convertRules( datalog->rulesOut() );
//        cout << "Converted Rules\n";
    };
    ~Database() {
    	clear();
    }
    
    string interpretStart();




};


#endif /* database_h */
