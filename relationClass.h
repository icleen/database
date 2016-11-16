//
//  relationClass.hpp
//  database
//
//  Created by Iain Lee on 10/24/16.
//  Copyright © 2016 Iain. All rights reserved.
//

#ifndef relationClass_h
#define relationClass_h

#include <stdio.h>
#include <vector>
#include <string>
#include <set>

#include "datalogProgram.h"

using namespace std;


class relation {
private:
    string name;
    vector<string> attributes;
    vector< vector<string> > tuples;
    
    int projectIndex(string attr);
    
    bool setFunction( vector< vector<string> > a, vector<string> b);
    string stringTuple( vector<string> t);

    bool isUnique( vector<string> t );

public:
    relation(string nme) : name(nme) {  };
    
    string nameOut() {
        return name;
    };
    int size() {
    	return tuples.size();
    };
    
    string attribute_at(int index);
    string tuple_at(int index1, int index2);
    vector<string> tuples_at(int index);

    void addAttribute(string attr);
    void addAttributes(vector<string> attrs);
    void addTuple(vector<string> tple);
    void addTuples(vector< vector<string> > tples);
    
    vector< vector<string> > tuplesOut() {
        return tuples;
    }
    vector<string> attributesOut() {
    	return attributes;
    }
    int attSize() {
    	return attributes.size();
    }
    
    string attsToString() {
    	stringstream ss;
    	for (int i = 0; i < attributes.size(); i++) {
    		ss << attributes[i] << " ";
    	}
    	ss << "\n";
    	return ss.str();
    }

    void changeName( string s ) {
    	name = s;
    }

    void selector( int a, int b );
    void selector( int a, string b );
    void selectSame();
    void projectUnique();
    void projector( vector<int> index);
    void renamer( relation* &rel );
    void rename( int attrIndex, string attrName);
    void sortTuples();

    void joinRelation(relation* rel);
    void conform( vector<ParameterClass*> params, string name );
};



#endif /* relationClass_h */
