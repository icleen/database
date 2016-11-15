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
        temp = conformToHead(head, temp);
	}else {
		for (int j = 1; j < tempRels.size(); j++) {
			temp = naturalJoin( temp, tempRels.at(j) );
		}
		temp = conformToHead( head, temp );
	}
	for (int i = 1; i < tempRels.size(); i++) {
		delete tempRels[i];
	}
	return temp;
}


relation* Database::naturalJoin( relation* &a, relation* &b ) {
//	cout << "naturalJoin\n";
	cleanProjList();
	totalAtts = 0;
	a->joinRelation( b );
	addToProjectList( a->attributesOut() );

	for (int i = 0; i < selectList.size(); i += 2) {
		int pop1 = selectList.front();
		selectList.pop();
		int pop2 = selectList.front();
		selectList.pop();
		selector( a, pop1, pop2 );
	}
	return a;
}


void Database::addToProjectList( const vector<string> &atts ) {
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


relation* Database::conformToHead( PredicateClass* &head,  relation* &rel ) {
	cleanProjList();
//	cout << "conform: " << rel->attsToString();
	vector<ParameterClass*> params = head->paramsOut();
	for (int i = 0; i < params.size(); i++) {
		myNode* n = new myNode();
		n->name = params[i]->toString();
		int num = nameLocation( n->name, rel );
		assert( num != -1 );
		n->index = num;
		projectList.push_back(n);
	}
	projector( rel );
	rel->changeName( head->nameOut() );
	return rel;
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











