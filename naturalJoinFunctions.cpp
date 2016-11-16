#include "database.h"



//
//	Making New Relations from the Rules
//

void Database::convertRules( const vector<RuleClass*> &rules ) {
//	cout << "ConvertRules\n";
	int relationSize = 0;
	int times = 0;
	do {
		relationSize = facts();
		for (int i = 0; i < rules.size(); i++) {
			relation* r = ruler( rules[i] );
			if (r != NULL) {
				int index = relationIndex( r->nameOut() );
				relations[index]->addTuples( r->tuplesOut() );
			}
			delete r;
		}
		times++;
	}while ( relationSize != facts() );
	cout << "Schemes populated after " << times << " passes through the Rules.\n";
	cleanProjList();
	renameOutput = "";
	for (int i = 0; i < relations.size(); i++) {
		relations[i]->sortTuples();
	}
}

relation* Database::ruler( RuleClass* rule ) {
//	cout << "Ruler\n";
	PredicateClass* head = rule->headOut();
//	cout << head->nameOut() << " ";
	vector< PredicateClass* > preds = rule->predicatesOut();
	vector< relation* > tempRels;

	for (int i = 0; i < preds.size(); i++) {
		string name = preds[i]->nameOut();
//		cout << name << endl;
	    int index = relationIndex( name );
	    if ( index >= 0 ) {
	    	relation* a = queryFind( relations[index],  preds[i] );
	    	tempRels.push_back(a);
	    }else {
	    	return NULL;
	    }
	}
	assert( tempRels.size() == preds.size() );
	if ( tempRels.empty() ) {
		return NULL;
	}
	relation* temp = tempRels.at(0);
	if (tempRels.size() == 1) {
		temp->conform( head->paramsOut(), head->nameOut() );
	}else {
		for (int j = 1; j < tempRels.size(); j++) {
			temp = naturalJoin( temp, tempRels.at(j) );
		}
		temp->conform( head->paramsOut(), head->nameOut() );
	}
	for (int i = 1; i < tempRels.size(); i++) {
		delete tempRels[i];
	}
	return temp;
}


relation* Database::naturalJoin( relation* &a, relation* &b ) {
//	cout << "naturalJoin\n";
	cleanProjList();
	cleanSelList();
	totalAtts = 0;
	a->joinRelation( b );
	addToProjectList( a->attributesOut() );

	while( !selectList.empty() ) {
		int pop1 = selectList.front();
		selectList.pop();
		int pop2 = selectList.front();
		selectList.pop();
		a->selector( pop1, pop2 );
	}
	return a;
}


void Database::addToProjectList( const vector<string> &atts ) {
//	cout << "Adding to project list\n";
	int i;
	bool found = false;
	for (i = 0; i < atts.size(); i++) {
		found = false;
		for (int j = 0; j < projectList.size(); j++) {
			if (projectList[j]->name == atts[i]) {
				selectList.push(j);
				selectList.push(i);
				found = true;
			}
		}
		if ( !found ) {
			myNode* n = new myNode();
			n->name = atts[i];
			n->index =  i;
			projectList.push_back(n);
		}
	}
}


bool Database::inProjectList( const string &name, const int &index ) {
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

int Database::nameLocation( const string &s,  relation* &r ) {
	for (int i = 0; i < r->attSize(); i++) {
		if (r->attribute_at(i) == s) {
			return i;
		}
	}
	return -1;
}

int Database::facts() {
	int amount = 0;
	for (int i = 0; i < relations.size(); i++) {
		amount += relations[i]->size();
	}
	return amount;
}











