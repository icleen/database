#include "database.h"

relation* Database::naturalJoin( relation* a, relation* b ) {
	projectList.clear();
	totalAtts = 0;
	relation* tempRel = joinAll( a, b );
	addToProjectList( tempRel->attributesOut() );

	for (int i = 0; i < selectList.size(); i += 2) {
		int a = selectList.front();
		selectList.pop();
		int b = selectList.front();
		selectList.pop();
		tempRel = selector( tempRel, a, b );
	}
	tempRel = projector( tempRel );

	return tempRel;
}


relation* Database::joinAll( relation* a, relation* b ) {
	cout << "join all\n";
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
	int i;
	for (i = 0; i < atts.size(); i++) {
		if ( !inProjectList( atts[i], i ) ) {
			myNode* n = new myNode();
			n->name = atts[i];
			n->index =  i;
			projectList.push_back(n);
		}
	}
	totalAtts += i + 1;

}

bool Database::inProjectList( string name, int index ) {
	for (int i = 0; i < projectList.size(); i++) {
		if (projectList[i]->name == name) {
			selectList.push(i);
			selectList.push(index);
			return true;
		}
	}
	return false;
}
