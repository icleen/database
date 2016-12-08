//
//  relationClass.cpp
//  database
//
//  Created by Iain Lee on 10/24/16.
//  Copyright © 2016 Iain. All rights reserved.
//

#include "relationClass.h"

string relation::attribute_at(int index) {
	return attributes[index];
}

string relation::tuple_at(int index1, int index2) {

	if ( index1 >= tuples.size() || index2 >= tuples[index1].size() ) {
		cout << "Bound error; that tuple does not exist ( relation::tuple_at() )\n";
		cout << index1 << " " << index2 << " " << tuples.size() << " " << tuples[index1].size() << endl;
		return "";
	}
	return tuples[index1][index2];
}

vector<string> relation::tuples_at(int index) {
	return tuples[index];
}

void relation::addAttribute(string attr) {
    attributes.push_back(attr);
}

void relation::addAttributes(vector<string> attrs) {
//	attributes = attrs;
	if (attributes.size() == 0) {
    	attributes = attrs;
    }else {
    	for (int i = 0; i < attrs.size(); i++) {
    		attributes.push_back(attrs[i]);
    	}
    }
}

void relation::addTuple(vector<string> tple) {
	if ( isUnique(tple) ) {
		tuples.push_back(tple);
	}
}

void relation::addTuplesPtr(vector< vector<string> >* tples) {

	for (int i = 0; i < tples->size(); i++) {
		if   (isUnique( tples->at(i) ) ) {
			tuples.push_back(tples->at(i));
		}
	}

}

void relation::addTuples(vector< vector<string> > tples) {

	for (int i = 0; i < tples.size(); i++) {
		if  ( isUnique( tples.at(i) ) ) {
			tuples.push_back(tples[i]);
		}
	}

}

bool relation::isUnique( vector<string> t ) {
	for (int i = 0; i < tuples.size(); i++) {
		if ( t == tuples.at(i) ) {
			return false;
		}
	}
	return true;
}

//
// Sorting Functions
//

void relation::sortTuples() {
//	cout << "Sort start\n";
	vector<string> tempTuples;
	string tup;
	int t;
	int j;
	vector<string> vtemp;
	string temp;
	for (int i = 0; i < tuples.size(); i++) {
		 tup = stringTuple( tuples[i] );
		 tempTuples.push_back(tup);
		 t = i;
		 j = tempTuples.size() - 2;
		 while ( j >= 0 && tup < tempTuples[j] ) { // while true, swap up
			 // swaps the temporary variables
			 temp = tempTuples[j];
			 tempTuples[j] = tempTuples[t];
			 tempTuples[t] = temp;
			 // swaps the actual tuples
			 vtemp = tuples[j];
			 tuples[j] = tuples[t];
			 tuples[t] = vtemp;
			 t = j;
			 j--;
		 }
	}
}

string relation::stringTuple( vector<string> t ) {
	stringstream ss;
	for (int i = 0; i < t.size(); i++) {
		ss << t[i];
	}
	return ss.str();
}
//
// Functions
//

void relation::selector( int a, int b ) {
    vector< vector<string> > new_tuples;
    for (int i = 0; i < tuples.size(); i++) {
        if (tuples[i][a] == tuples[i][b]) {
            new_tuples.push_back(tuples[i]);
        }
    }
    tuples = new_tuples;
}
void relation::selector( int a, string b ) {
    vector< vector<string> > new_tuples;
    for (int i = 0; i < tuples.size(); i++) {
        if (tuples[i][a] == b) {
            new_tuples.push_back(tuples[i]);
        }
    }
    tuples = new_tuples;
}

void relation::projector( vector<int> index ) {
//    cout << "Projecting\n";
	vector< vector<string> > temp;
    for (int i = 0; i < tuples.size(); i++) {
    	vector<string> tple;
    	for (int j = 0; j < index.size(); j++) {
    		tple.push_back( tuples[i][ index[j] ] );
    	}
    	temp.push_back( tple );
    }
    tuples = temp;
//    cout << "Done\n";
}

int relation::projectIndex(string attr) {
    for (int i = 0; i < attributes.size(); i++) {
        if (attr == attributes[i]) {
            return i;
        }
    }
    return -1;
}

bool relation::setFunction( vector< vector<string> >& a, vector<string>& b) {
	for (int i = 0; i < a.size(); i++) {
		if ( a[i] == b ) {
			return false;
		}
	}
	return true;
}

void relation::rename( int attrIndex, string attrName) {
	if ( attrIndex >= attributes.size() ) {
		attributes.push_back(attrName);
//		cout << "Pushed it back\n";
		return;
	}
    attributes[attrIndex] = attrName;
}


void relation::joinRelation( relation* rel ) {
//	cout << "joining relations\n";
	vector< vector<string> > newTuples;
	vector< vector<string> >* other = rel->tuplesPtr();
	vector<string> tempTuple;
	for (int i = 0; i < tuples.size(); i++) {
		for (int j = 0; j < other->size(); j++) {
			tempTuple = tuples[i];
			for (int k = 0; k < other->at(j).size(); k++) {
				tempTuple.push_back( other->at(j).at(k) );
			}
			newTuples.push_back( tempTuple );
			tempTuple.clear();
		}
	}
	tuples = newTuples;
//	cout << "adding Attrs\n";
	addAttributes( rel->attributesOut() );
//	cout << "Done\n";
}

void relation::conform( vector<ParameterClass*>* params, string nme ) {
//	cout << "conforming\n";
	name = nme;
	vector<int> project;
	bool found;
	string s;
	for (int i = 0; i < params->size(); i++) {
		s = params->at(i)->toString();
		found = false;
		for (int j = 0; j < attributes.size(); j++) {
			if ( attributes[j] == s && !found ) {
				project.push_back(j);
				found = true;
			}
		}
	}
	projector( project );
//	cout << "Done\n";
}

void relation::selectSame() {
	for (int i = 0; i < attributes.size(); i++) {
		for (int j = i; j < attributes.size(); j++) {
			if ( attributes.at(i) == attributes.at(j) ) {
				selector( i, j );
			}
		}
	}
}

void relation::projectUnique() {
	vector<int> project;
	bool found;
	string s;
	for (int i = 0; i < attributes.size(); i++) {
		s = attributes[i];
		found = false;
		for (int j = i + 1; j < attributes.size(); j++) {
			if ( attributes[j] == s && !found ) {
				project.push_back(j);
				found = true;
			}
		}
	}
	projector( project );
}






