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
		for (int j = 0; j < rules.size(); j++) {
			preds = rules.at(j)->predicatesOut();
			for (int k = 0; k < preds.size(); k++) {
				if ( head->nameOut() == preds[k]->nameOut() ) {
					graph->makeEdge( j, i );	//	make and edge going from the rule where the predicate matching the rules[i] was found, ie node[j], to node[i]
				}
			}
		}
	}
}

void Database::makeNodes( int size ) {

}
