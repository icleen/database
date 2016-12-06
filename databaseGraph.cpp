#include "database.h"

void Database::makeGraph( const vector<RuleClass*> &rules ) {
	graph = new graphClass();
	graph->makeNodes( rules.size() );
	vector<PredicateClass*> preds;
	PredicateClass* head;
	/*
	 * For each rule on the left (rules[i]), look through each other rule including itself (rules[j]),
	 * and scan those rules' predicate names (preds[k]) to see if any of them match the name of the rule being investigated (rules[i]).
	 * If they match, create an edge in the graph going from the second rule (rules[j]) to the rule being investigated (rules[i])
	 */
	for (int i = 0; i < rules.size(); i++) {
		head = rules[i]->headOut();
		graph->addKey( head->nameOut() );
		for (int j = 0; j < rules.size(); j++) {
			preds = rules.at(j)->predicatesOut();
			for (int k = 0; k < preds.size(); k++) {
				if ( head->nameOut() == preds[k]->nameOut() ) {
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
	vector< vector<int> >  g = graph->SCCindex();
//	cout << "SCC graph:\n" << graphOut( g );
	vector<RuleClass*> rules = datalog->rulesOut();
	assert( g.size() == rules.size() );
	vector<RuleClass*> use;
	vector<int> ruleIndex;
	for (int i = 0; i < g.size(); i++) {
		stringstream ss;
		if ( 1 < g.at(i).size() ) {
			for (int j = 0; j < g.at(i).size(); j++) {
				use.push_back( rules.at( g.at(i).at(j) ) );
				ruleIndex.push_back( g.at(i).at(j) );
			}
			vectorSort( ruleIndex );
			for (int k = 0; k < ruleIndex.size(); k++) {
				ss << "R" << ruleIndex.at(k);
				if ( k < g.at(i).size()- 1) {
					ss << ",";
				}
			}
			cout << convertRules( use ) << " passes: " << ss.str() << endl;
			use.clear();
		}else if ( !g.at(i).empty() ){
			if ( !graph->isTrivial( g.at(i).at(0) ) ) {
				use.push_back( rules.at( g.at(i).at(0) ) );
				cout << convertRules( use ) << " passes: R" << g.at(i).at(0) << endl;
			}else {
				relation* r = ruler( rules.at( g.at(i).at(0) ) );
				if (r != NULL) {
					int index = relationIndex( r->nameOut() );
					relations[index]->addTuplesPtr( r->tuplesPtr() );
				}
				delete r;
				cout << 1 << " passes: R" << g.at(i).at(0) << endl;
				use.clear();
			}
		}
	}
	for (int i = 0; i < relations.size(); i++) {
		relations[i]->sortTuples();
	}
}

void Database::vectorSort( vector<int> &a ) {
	int x;
	int temp;
	for (int i = 0; i < a.size(); i++) {
		x = i;
		for (int j = i; j < a.size(); j++) {
			if ( a[x] > a[j] ) {
				temp = a[j];
				a[j] = a[x];
				a[x] = temp;
				x = j;
			}
		}
	}
}

