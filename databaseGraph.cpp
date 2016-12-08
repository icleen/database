#include "database.h"

void Database::makeGraph( const vector<RuleClass*> *rules ) {
	graph = new graphClass();
	graph->makeNodes( rules->size() );
	vector<PredicateClass*>* preds;
	PredicateClass* head;
	/*
	 * For each rule on the left (rules[i]), look through each other rule including itself (rules[j]),
	 * and scan those rules' predicate names (preds[k]) to see if any of them match the name of the rule being investigated (rules[i]).
	 * If they match, create an edge in the graph going from the second rule (rules[j]) to the rule being investigated (rules[i])
	 */
	for (int i = 0; i < rules->size(); i++) {
		head = rules->at(i)->headOut();
		graph->addKey( head->nameOut() );
		for (int j = 0; j < rules->size(); j++) {
			preds = rules->at(j)->predicatesOut();
			for (int k = 0; k < preds->size(); k++) {
				if ( head->nameOut() == preds->at(k)->nameOut() ) {
					graph->makeEdge( j, i );	//	make and edge going from the rule where the predicate matching the rules[i] was found, ie node[j], to node[i]
				}
			}
		}
	}
	cout << "Dependency Graph\n" << graphOut( graph->graphOut() );
}

string Database::graphOut( vector< vector<int> > gr ) {
	stringstream out;
//	out << "Dependency graph:\n";
	for (int i = 0; i < gr.size(); i++) {
		out << "R" << i << ":";
		for (int j = 0; j < gr.at(i).size(); j++) {
			out << "R" << gr.at(i).at(j);
			if (j < gr.at(i).size() - 1) {
				out << ",";
			}
		}
		out << endl;
	}
	out << endl;
	return out.str();
}

void Database::optimizedRules() {
//	cout << "ConvertRules\n";
	vector< set<int> >  g = graph->SCCindex();
//	cout << "SCC graph:\n" << graphOut( g );
	vector<RuleClass*>* rules = datalog->rulesOut();
	assert( g.size() == rules->size() );
	vector<RuleClass*> use;
	set<int>::iterator it;
	for (int i = 0; i < g.size(); i++) {
		stringstream ss;
		it = g.at(i).begin();
		if ( 1 < g.at(i).size() ) {
			for (it = g.at(i).begin(); it!=g.at(i).end(); ++it) {
				use.push_back( rules->at( *it ) );
				ss << *it;
			}
			cout << convertRules( use ) << " passes: " << ss.str() << endl;
		}else if ( !g.at(i).empty() ){
			if ( !graph->isTrivial( *it ) ) {
				use.push_back( rules->at( *it ) );
				cout << convertRules( use ) << " passes: R" << *it << endl;
			}else {
				relation* r = ruler( rules->at( *it ) );
				if (r != NULL) {
					int index = relationIndex( r->nameOut() );
					relations[index]->addTuplesPtr( r->tuplesPtr() );
				}
				delete r;
				cout << 1 << " passes: R" << *it << endl;
			}
		}
		use.clear();
	}
	sort();
}

