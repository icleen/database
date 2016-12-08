#include "database.h"



//
//	Making New Relations from the Rules
//

int Database::convertRules( const vector<RuleClass*> &rules ) {
//	cout << "ConvertRules\n";
	int relationSize = 0;
	int times = 0;
	do {
		relationSize = facts();
		for (int i = 0; i < rules.size(); i++) {
			relation* r = ruler( rules[i] );
			if (r != NULL) {
				int index = relationIndex( r->nameOut() );
				relations[index]->addTuplesPtr( r->tuplesPtr() );
			}
			delete r;
		}
		times++;
	}while ( relationSize != facts() );
//	cout << "Schemes populated after " << times << " passes through the Rules.\n";
	renameOutput = "";
	sort();
	return times;
}

relation* Database::ruler( RuleClass* rule ) {
//	cout << "Ruler\n";
	PredicateClass* head = rule->headOut();
//	cout << head->nameOut() << " ";
	vector< PredicateClass* >* preds = rule->predicatesOut();
	vector< relation* > tempRels;

	for (int i = 0; i < preds->size(); i++) {
		string name = preds->at(i)->nameOut();
//		cout << name << endl;
	    int index = relationIndex( name );
	    if ( index >= 0 ) {
	    	relation* a = queryFind( relations[index],  preds->at(i) );
	    	tempRels.push_back(a);
	    }else {
	    	return NULL;
	    }
	}
	assert( tempRels.size() == preds->size() );
	if ( tempRels.empty() ) {
		return NULL;
	}
	relation* temp = tempRels.at(0);
	if (tempRels.size() == 1) {
		temp->conform( head->paramsOut(), head->nameOut() );
	}else {
		for (int j = 1; j < tempRels.size(); j++) {
			temp->joinRelation( tempRels.at(j) );
			temp->selectSame();
		}
		temp->conform( head->paramsOut(), head->nameOut() );
	}
	for (int i = 1; i < tempRels.size(); i++) {
		delete tempRels[i];
	}
	return temp;
}

int Database::facts() {
	int amount = 0;
	for (int i = 0; i < relations.size(); i++) {
		amount += relations[i]->size();
	}
	return amount;
}









