#include "database.h"



//
//	Making New Relations from the Rules
//

void Database::convertRules( vector<RuleClass*> rules ) {

	for (int i = 0; i < rules.size(); i++) {
		cout << "convert: " << i << endl;
		relation* r = ruler( rules[i] );
		cout << r->nameOut() << endl;
		relations.push_back( r );
		vector< vector<string> > tuples = r->tuplesOut();
		for (int i = 0; i < tuples.size(); i++) {
			for (int j = 0; j < tuples[i].size(); j++) {
				cout << tuples[i][j] << " ";
			}
			cout << endl;
		}
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
//	    	cout << "\nAttributes: "<< a->attribute_at(0) << a->attribute_at(1) << endl;
	    	tempRels.push_back(a);
	    }else {
	    	return NULL;
	    }
	}
	assert( tempRels.size() == preds.size() );
	if ( tempRels.empty() ) {
		return NULL;
	}
	relation* temp;
	for (int j = 1; j < tempRels.size(); j++) {
		temp = naturalJoin( tempRels.at(0), tempRels.at(j) );
	}
	temp = conformToHead( head, temp );
//	vector< vector<string> > tuples = temp->tuplesOut();
//	cout << endl;
//	for (int i = 0; i < tuples.size(); i++) {
//		for (int j = 0; j < tuples[i].size(); j++) {
//			cout << tuples[i][j] << " ";
//		}
//		cout << endl;
//	}
	return temp;
}


relation* Database::naturalJoin( relation* a, relation* b ) {
	projectList.clear();
	totalAtts = 0;
	relation* tempRel = joinAll( a, b );
	addToProjectList( tempRel->attributesOut() );

	for (int i = 0; i < selectList.size(); i += 2) {
		int pop1 = selectList.front();
		selectList.pop();
		int pop2 = selectList.front();
		selectList.pop();
		tempRel = selector( tempRel, pop1, pop2 );
	}
//	vector< vector<string> > tuples = tempRel->tuplesOut();
//	cout << endl;
//	for (int i = 0; i < tuples.size(); i++) {
//		for (int j = 0; j < tuples[i].size(); j++) {
//			cout << tuples[i][j] << " ";
//		}
//		cout << endl;
//	}
//	tempRel = projector( tempRel );
//	tuples.clear();
//	tuples = tempRel->tuplesOut();
//	cout << endl;
//	for (int i = 0; i < tuples.size(); i++) {
//		for (int j = 0; j < tuples[i].size(); j++) {
//			cout << tuples[i][j] << " ";
//		}
//		cout << endl;
//	}
	return tempRel;
}


relation* Database::joinAll( relation* a, relation* b ) {
//	cout << "join all\n";
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
	rel->addAttributes( a->attributesOut() );
	rel->addAttributes( b->attributesOut() );
	return rel;
}


void Database::addToProjectList( vector<string> atts ) {
//	cout << "Adding to project list\n";
	int i;
	for (i = 0; i < atts.size(); i++) {
		if ( !inProjectList( atts[i], i ) ) {
			myNode* n = new myNode();
			n->name = atts[i];
			n->index =  i;
//			cout << "AddtoProjlist: " << n->name << " " << n->index << endl;
			projectList.push_back(n);
		}
	}
	totalAtts += i + 1;

}


bool Database::inProjectList( string name, int index ) {
//	cout << "Checking if it's in project list\n";
	for (int i = 0; i < projectList.size(); i++) {
		if (projectList[i]->name == name) {
//			cout << "inprojlist: " << name << endl;
			selectList.push(i);
			selectList.push(index);
			return true;
		}
	}
	return false;
}

relation* Database::conformToHead( PredicateClass* head, relation* &rel ) {
	projectList.clear();
//	cout << "conform: " << rel->attsToString();
	vector<ParameterClass*> params = head->paramsOut();
	for (int i = 0; i < params.size(); i++) {
		myNode* n = new myNode();
		n->name = params[i]->toString();
		n->index = nameLocation( n->name, rel );
		projectList.push_back(n);
	}
	rel = projector( rel );
	reorder( rel );
	rel->changeName( head->nameOut() );
	return rel;
}

int Database::nameLocation( string s, relation* r ) {
	for (int i = 0; i < r->attSize(); i++) {
		if (r->attribute_at(i) == s) {
			return i;
		}
	}
	return -1;
}



void Database::reorder( relation* &rel ) {
	cout << "reorder: " << rel->attsToString();

}














