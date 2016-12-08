#include "database.h"


void Database::makeRelations() {
    
    vector<class PredicateClass*>* schemes = datalog->schemesOut();
    for (int i = 0; i < schemes->size(); i++) {
        relation* rel = new relation(schemes->at(i)->nameOut());
        vector<class ParameterClass*>* params = schemes->at(i)->paramsOut();
        for (int j = 0; j < params->size(); j++) {
            rel->addAttribute(params->at(j)->toString());
        }
        relations.push_back(rel);
    }
    makeTuples();
}

void Database::makeTuples() {
    
    vector<class PredicateClass*>* facts = datalog->factsOut();
    for (int i = 0; i < facts->size(); i++) {
        int index = relationIndex(facts->at(i)->nameOut());
        if (index == -1) {
//            cout << "Fact matches no known relations\n";
            return;
        }
        relations[index]->addTuple( makeTuple( facts->at(i)->paramsOut() ) );
    }
    
}

vector<string> Database::makeTuple(vector<class ParameterClass*>* params) {
    
    vector<string> tple;
    for (int k = 0; k < params->size(); k++) {
        tple.push_back(params->at(k)->toString());
    }
    return tple;
    
}

// finds the index of a given relation based off of name
int Database::relationIndex(string name) {
    for (int i = 0; i < relations.size(); i++) {
        if (relations[i]->nameOut() == name) {
            return i;
        }
    }
    return -1;
}

// makes a copy of a given relation
relation* Database::copyRelation(relation* a) {
    
    relation* b = new relation(a->nameOut());
    vector< vector<string> >* tempTuple = a->tuplesPtr();
    for (int i = 0; i < tempTuple->size(); i++) {
        b->addTuple( tempTuple->at(i) );
    }
    b->addAttributes( a->attributesOut() );
    return b;
}

// returns the index of any already existing vars in the project list
int Database::repeatVar(string var) {

    for (int i = 0; i < projectNames.size(); i++) {
        if (projectNames[i] == var) {
            return i;
        }
    }
    return -1;
}

//
// Interpreting the data
//

string Database::interpretStart() {
	stringstream out;
    vector<class PredicateClass*>* queries = datalog->queriesOut();
    vector<relation*> tempRelations;
    relation* tempRel;
    for (int i = 0; i < queries->size(); i++) {
    	out << queries->at(i)->toString() << "?";
//    	cout << "(" << i + 1 << ") Working...\n";
    	int index = relationIndex( queries->at(i)->nameOut() );
    	if ( index < 0 ) {
    		out << " No";
    	}else {
        	tempRel = queryFind( relations[index], queries->at(i) );
    //    	cout << "QueryFind?\n";
        	if ( tempRel->size() == 0 ) {
        		out << " No";
        	}else {
        		out << " Yes("<< tempRel->size() << ")";
        		out << renameOutput;
    //    		cout << "rename?\n";
        	}
    	}
    	out << "\n";
    	delete tempRel;
    }

    return out.str();
}

string Database::relationsOut() {
	stringstream out;
	for (int i = 0; i < relations.size(); i++) {
		out << relations[i]->nameOut() << " " << relations[i]->attribute_at(0) << " ";
	}
	out << endl;
	return out.str();
}


// return a new relation based on the given relation and the data in the given query
relation* Database::queryFind(relation *relat, PredicateClass *query) {
//	cout << relationsOut();
	projectNames.clear();
	projectIndex.clear();
	renameOutput = "";
    relation* tempRel = copyRelation(relat);
//    cout << "Here?\n";
    vector<ParameterClass*>* params = query->paramsOut();
    for (int i = 0; i < params->size(); i++) {
//    	cout << "(" << i + 1 << ") Working...\n";
        if (params->at(i)->typeOut() == "DOM") {
        	// select on the value
        	tempRel->selector( i, params->at(i)->toString() );
        }else {
        	// decide whether the VAR is already in the project list
            int  repeatIndex = repeatVar( params->at(i)->toString() );
            if (repeatIndex != -1) {
            	//deal with a repeated variable
            	tempRel->selector( i, projectIndex[repeatIndex] );
            }else {
            	// add to the project list for later projection and renaming
            	projectNames.push_back( params->at(i)->toString() );
            	projectIndex.push_back( i );
            }
        }
    }
    projector( tempRel );
//    cout << "Renamer prob\n";
    renamer( tempRel );
//    cout << "Not\n";
    return tempRel;
}


// returns the new relation based on the projections listed in the projectList
void Database::projector(relation* &rel) {
//	rel->sortTuples();
	if ( projectIndex.empty() ) {
		return;
	}
	rel->projector( projectIndex );
}


// returns a string with the relation info in it
void Database::renamer(relation* &rel) {
	if ( projectNames.size() == 0) {
		return;
	}
    for ( int j = 0; j < projectNames.size(); j++ ) {
        rel->rename( j, projectNames.at(j) );
    }
//    cout << "renamed: " << rel->attsToString() << endl;
	
	renameOutputFunc(rel, projectNames.size());
}

void Database::renameOutputFunc(relation* rel, int b) {
    stringstream out;
    out <<"\n";
    for (int i = 0; i < rel->size(); i++) {
        out << "  ";
        for ( int j = 0; j < b; j++ ) {
            out << rel->attribute_at(j) << "=" << rel->tuple_at( i, j);
            if ( j < projectNames.size() - 1 ) {
                out << ", ";
            }
        }
        if ( i < rel->size() - 1 ) {
            out << "\n";
        }
    }
    renameOutput = out.str();
}

void Database::clear() {
	for (int i = 0; i < relations.size(); i++) {
		delete relations[i];
	}
	delete graph;
}

void Database::sort() {
	for (int i = 0; i < relations.size(); i++) {
		relations[i]->sortTuples();
	}
}








