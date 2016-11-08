#include "database.h"


void Database::makeRelations() {
    
    vector<class PredicateClass*> schemes = datalog->schemesOut();
    for (int i = 0; i < schemes.size(); i++) {
        relation* rel = new relation(schemes[i]->nameOut());
        vector<class ParameterClass*> params = schemes[i]->paramsOut();
        for (int j = 0; j < params.size(); j++) {
            rel->addAttribute(params[j]->toString());
        }
        relations.push_back(rel);
    }
    makeTuples();
}

void Database::makeTuples() {
    
    vector<class PredicateClass*> facts = datalog->factsOut();
    for (int i = 0; i < facts.size(); i++) {
        int index = relationIndex(facts[i]->nameOut());
        if (index == -1) {
            cout << "Fact matches no known relations\n";
            return;
        }
        relations[index]->addTuple(makeTuple(facts[i]->paramsOut()));
    }
    
}

vector<string> Database::makeTuple(vector<class ParameterClass*> params) {
    
    vector<string> tple;
    for (int k = 0; k < params.size(); k++) {
        tple.push_back(params[k]->toString());
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
    vector< vector<string> > tempTuple = a->tuplesOut();
    for (int i = 0; i < tempTuple.size(); i++) {
        b->addTuple(tempTuple[i]);
    }
    b->addAttributes( a->attributesOut() );
    return b;
}

// returns the index of any already existing vars in the project list
int Database::repeatVar(string var) {

    for (int i = 0; i < projectList.size(); i++) {
        if (projectList[i]->name == var) {
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

    vector<class PredicateClass*> queries = datalog->queriesOut();
    vector<relation*> tempRelations;
    relation* tempRel;
    for (int i = 0; i < queries.size(); i++) {
    	out << queries[i]->toString() << "?";
//    	cout << "(" << i + 1 << ") Working...\n";
    	int index = relationIndex( queries[i]->nameOut() );
    	if ( index < 0 ) {
    		out << " No";
    	}else {
        	tempRel = queryFind( relations[index], queries[i] );
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
    }

    return out.str();
}


// return a new relation based on the given relation and the data in the given query
relation* Database::queryFind(relation *relat, PredicateClass *query) {
	projectList.clear();
    relation* tempRel = copyRelation(relat);
//    cout << "Here?\n";
    vector<ParameterClass*> params = query->paramsOut();
    for (int i = 0; i < params.size(); i++) {
//    	cout << "(" << i + 1 << ") Working...\n";
        if (params[i]->typeOut() == "DOM") {
        	// select on the value
        	tempRel = selector( tempRel, i, params[i]->toString() );
        }else {
        	// decide whether the VAR is already in the project list
            int  repeatIndex = repeatVar( params[i]->toString() );
            if (repeatIndex != -1) {
            	//deal with a repeated variable
                tempRel = selector( tempRel, i, projectList[repeatIndex]->index );
            }else {
            	// add to the project list for later projection and renaming
            	myNode* data = new myNode();
            	data->name = params[i]->toString();
            	data->index = i;
            	projectList.push_back(data);
            }
        }
    }
    tempRel = projector(tempRel);
//    cout << "Renamer prob\n";
    renamer( tempRel );
//    cout << "Not\n";
    return tempRel;
}

// returns the new relation based off of the select function
relation* Database::selector(relation* r, int a, int b) {
	 relation* tempRel = new relation( r->nameOut() );
	tempRel->addTuples( r->select( a, b ) );
	tempRel->addAttributes( r->attributesOut() );
	return tempRel;
}

// returns the new relation based off of the select function
relation* Database::selector(relation* r, int a, string b) {
	relation* tempRel = new relation( r->nameOut() );
	tempRel->addTuples( r->select( a, b ) );
	tempRel->addAttributes( r->attributesOut() );
	return tempRel;
}

// returns the new relation based on the projections listed in the projectList
relation* Database::projector(relation* rel) {
	rel->sortTuples();
	if ( projectList.empty() ) {
		return rel;
	}
	relation* tempRel = new relation( rel->nameOut() );
	vector<int> indexi;
	for (int i = 0; i < projectList.size(); i++) {
		indexi.push_back( projectList[i]->index );
	}
	tempRel->addTuples( rel->project( indexi ) );
	tempRel->addAttributes( rel->attributesOut() );
	return tempRel;
}

// returns a string with the relation info in it
void Database::renamer(relation* &rel) {
	if ( projectList.size() == 0) {
		return;
	}
	stringstream out;
	out <<"\n";
	for (int i = 0; i < rel->size(); i++) {
		out << "  ";
		for ( int j = 0; j < projectList.size(); j++ ) {
			string s = projectList[j]->name;
//			cout << "Rename function?\n";
			rel->rename( i, s );
//			cout << "Not?\n";
			out << rel->attribute_at(i) << "=" << rel->tuple_at( i, j);
			if ( j < projectList.size() - 1 ) {
				out << ", ";
			}
		}
		if ( i < rel->size() - 1 ) {
			out << "\n";
		}
	}

	renameOutput = out.str();
}



//
//	Making New Relations from the Rules
//

void Database::convertRules( vector<RuleClass*> rules ) {

//	while (  ) // while the relations.size() is not the same as last time we called this function

	for (int i = 0; i < rules.size(); i++) {
		ruler( rules[i] );
	}

}

relation* Database::ruler( RuleClass* rule ) {

	PredicateClass* head = rule->headOut();
	vector< PredicateClass* > preds = rule->predicatesOut();
	vector< relation* > tempRels;

	for (int i = 0; i < preds.size(); i++) {
		string name = preds[i]->nameOut();
	    int index = relationIndex( name );
	    if ( index >= 0 ) {
	    	relation* a = queryFind( relations[index],  preds[i] );
	    	tempRels.push_back(a);
	    }else {
	    	return NULL;
	    }
	}
	if ( tempRels.empty ) {
		return NULL;
	}
	relation* rel = tempRels[0];
	for (int j = 1; j < tempRels.size(); j++) {
		rel = naturalJoin( tempRels.at(0), tempRels.at(j) );
	}

	return rel;
}

relation* Database::naturalJoin( relation* a, relation* b ) {
	relation* tempRel = new relation( a->nameOut() );
	int common = 0;
	for (int i = 0; i < a->attSize(); i++) {
		for (int j = 0; j < b->attSize(); j++) {
			if ( a->attribute_at(i) == b->attribute_at(j) ) {
//					add the common attribute to the new relation
				common++;
			}
		}
	}
	if (common == 0) {
		//	none in common case
		tempRel = noCommon( a, b );
	}else if ( common == a->size() || common == b->size() ) {
		// all of them are in common for at least one relation
		tempRel = allCommon( a, b );
	}else {
		// some are in common, but not all
		tempRel = someCommon( a, b );
	}

	return tempRel;
}

relation* Database::noCommon(relation* a, relation* b) {
	vector< vector<string> > Atuples = a->tuplesOut();
	vector< vector<string> > Btuples = b->tuplesOut();
	vector< vector<string> > tuples;

	for (int i = 0; i < Atuples.size(); i++) {
		for (int j = 0; j < Btuples.size(); j++) {
			vector<string> tempTuple = Atuples[i];
			for (int k = 0; k < Btuples[j].size(); k++) {
				tempTuple.push_back( Btuples[j][k] );
			}
			tuples.push_back( tempTuple );
		}
	}

	relation* rel = new relation( a->nameOut() );
	rel->addTuples( tuples );
	return rel;
}

relation* Database::someCommon(relation* a, relation* b) {

	vector< vector<string> > Atuples = a->tuplesOut();
	vector< vector<string> > Btuples = b->tuplesOut();
	vector< vector<string> > tuples;

	for (int i = 0; i < Atuples.size(); i++) {
		for (int j = 0; j < Btuples.size(); j++) {
			vector<string> tempTuple = Atuples[i];
			for (int k = 0; k < Btuples[j].size(); k++) {
				tempTuple.push_back( Btuples[j][k] );
			}
			tuples.push_back( tempTuple );
		}
	}

	relation* rel = new relation( a->nameOut() );
	rel->addTuples( tuples );
	return rel;

}

relation* Database::allCommon(relation* a, relation* b) {



}
















