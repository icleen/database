#include "graphClass.h"

vector< vector<int> > graphClass::graphOut() {
	vector< vector<int> > gr;
	for (int i = 0; i < nodes.size(); i++) {
		gr.push_back( nodes.at(i)->getEdges() );
	}
	return gr;
}

void graphClass::clear() {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	nodes.clear();
}

void graphClass::resetVisit() {
	for (int i = 0; i < nodes.size(); i++) {
		nodes.at(i)->visited = false;
	}
	clearStack();
}

void graphClass::clearStack() {
	while ( !mystack.empty() ) {
		mystack.pop();
	}
}

void graphClass::addKey( string ky ) {
	key.push_back( ky );
}

void graphClass::addKeys( vector<string> ky ) {
	key = ky;
}

//void graphClass::importFromFile( string fileName ) {
//	ifstream file;
//	file.open( fileName.c_str() );
//	if( file.fail() ) {
//		cout << "File not found!" << endl;
//		return;
//	}
//	string inodes;
//	getline( file, inodes );
//	key = inodes;
//	makeNodes( inodes.size() );
//	string iedge;
//	int a, b;
//	while( getline( file, iedge ) ) {
//		assert( iedge.size() == 2 );
//		a = 0;
//		b = 0;
//		for (int j = 0; j < inodes.size(); j++) {
//			if ( iedge.at(0) == inodes.at(j) ) {
//				a = j;
//			}
//			if ( iedge.at(1) == inodes.at(j) ) {
//				b = j;
//			}
//		}
//		makeEdge( a, b );
//	}
////	cout << id << endl;
//	file.close();
//}

void graphClass::makeNodes( int amount ) {
	for (int i = 0; i < amount; i++) {
		gnode* gn = new gnode( id++ );
		nodes.push_back(gn);
	}
}

void graphClass::makeEdge( int nodeId, int toptr ) {
	assert( toptr < nodes.size() );
	gnode* gn = nodes.at(toptr);
	bool found = false;
	for (int i = 0; i < nodes.at(nodeId)->edges.size(); i++) {
		if ( nodes.at(nodeId)->edges.at(i)->id == gn->id ) {
			found = true;
		}
	}
	if (!found) {
		nodes.at(nodeId)->edges.push_back(gn);
	}
}

string graphClass::traverse( int id ) {
	if ( nodes.at(id)->visited == true ) {
		return "";
	}
	nodes.at(id)->visited = true;
	stringstream value;
	for (int i = 0; i < nodes.at(id)->edges.size(); i++) {
		if ( !nodes.at(id)->edges.at(i)->visited ) {
			value << traverse( nodes.at(id)->edges.at(i)->id );
		}
	}
	mystack.push( nodes.at(id)->id );
	value << nodes.at(id)->id;
	return value.str();
}

string graphClass::pathOutput( string path ) {
	stringstream output;
	stringstream ss;
	int index = -1;
	for (int i = path.size() - 1; i >= 0; i--) {
		ss.clear();
		ss << path.at(i);
		ss >> index;
		output << key.at( index );
	}
	return output.str();
}

string graphClass::pathOutput( stack<int> stck ) {
	stringstream output;
	while ( !stck.empty() ) {
		output << key.at( stck.top() );
		stck.pop();
	}
	return output.str();
}

string graphClass::pathIndexOutput( stack<int> stck ) {
	stringstream output;
	while ( !stck.empty() ) {
		output << stck.top();
		stck.pop();
	}
	return output.str();
}

stack<int> graphClass::POT() {
//	resetVisit();
//	clearStack();
	for (int i = 0; i < nodes.size(); i++) {
		if ( !nodes.at(i)->visited ) {
//			cout << i + 1 << " times\n";
			traverse( nodes.at(i)->id );
		}
	}
	return mystack;
}

vector<string> graphClass::DFS( stack<int> stck ) {
	vector<string> components;
//	resetVisit();
	while ( !stck.empty() ) {
		traverse( stck.top() );
		string s = pathOutput( mystack );
		if ( s != "" ) {
			components.push_back( s );
		}
		clearStack();
		stck.pop();
	}
	return components;
}

vector< vector<int> > graphClass::DFSindex( stack<int> stck ) {
	vector< vector<int> > output;
	vector<int> components;
	while ( !stck.empty() ) {
		traverse( stck.top() );
		while ( !mystack.empty() ) {
			components.push_back( mystack.top() );
			mystack.pop();
		}
		output.push_back( components );
		components.clear();
		stck.pop();
	}
	return output;
}

string graphClass::DFSOutput( vector<string> dfs ) {
	stringstream output;
//	resetVisit();
//	output << "These are the strongly connected components:\n";
	for (int i = 0; i < dfs.size(); i++) {
		output << dfs.at(i) << endl;
	}
	return output.str();
}

graphClass* graphClass::reverse() {
	graphClass* ng = new graphClass();
	ng->makeNodes( key.size() );
	ng->addKeys( key );
	int a,b;
	for (int i = 0; i < nodes.size(); i++) {
		a = 0;
		b = nodes.at(i)->id;
		for (int j = 0; j < nodes.at(i)->edges.size(); j++) {
			a = nodes.at(i)->edges.at(j)->id;
			ng->makeEdge( a , b );
		}
	}
	return ng;
}


string graphClass::SCC() {
	graphClass* rvrse = reverse();
	string s = DFSOutput( DFS( rvrse->POT() ) );
	delete rvrse;
	return s;
}

vector< vector<int> > graphClass::SCCindex() {
	graphClass* rvrse = reverse();
	vector< vector<int> > g = DFSindex( rvrse->POT() );
	delete rvrse;
	return g;
}

bool graphClass::isTrivial( int index ) {
	assert( index < nodes.size() );
	for (int i = 0; i < nodes.at(index)->edges.size(); i++) {
		if ( nodes.at(index)->edges.at(i)->id == nodes.at(index)->id ) {
//			cout << "yes\n";
			return false;
		}
	}
	return true;
}






