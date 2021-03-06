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
    vector<string> makeTuple(vector<class ParameterClass*> params);
    
    // interpretation functions:
    
    relation* queryFind(relation* relat, PredicateClass* query);
    void selector(relation* &r, int a, int b);
    void selector(relation* &r, int a, string b);
    void projector(relation* &r);
    void renamer(relation* &rel);
    vector<myNode*> projectList;
    
    int relationIndex(string name);
    relation* copyRelation(relation* a);
    int repeatVar(string var);
    
    // Lab 4 functions:
    void convertRules( const vector<RuleClass*> &rules );
    relation* ruler( RuleClass* rule );
    relation* naturalJoin( relation* &a, relation* &b );
    relation* joinAll( relation* &a, relation* &b );
    vector< vector<string> > joinedTuples( vector< vector<string> > Atuples, vector< vector<string> > Btuples );
    void addToProjectList( const vector<string> &s );
    bool inProjectList( const string &name, const int &index );
    relation* conformToHead( PredicateClass* &head, relation* &r );
    void reorder( relation* &rel );
    int nameLocation( const string &s, relation* &r );
    int totalAtts;
    queue<int> selectList;

    string relationsOut();
    void renameOutputFunc(relation* rel, int b);
    int facts();

    void cleanProjList();
    void clear();

public:
    Database(datalogClass* data) : datalog(data) {
        makeRelations();
        convertRules( datalog->rulesOut() );
    };
    ~Database() {
    	clear();
    }
    
    string interpretStart();




};


#endif /* database_h */
