#include "FDD.h"
#include <algorithm>
#include <map>

FunctionalDependence::FunctionalDependence(int n, int s):dims(n), size(s) {
}

void FunctionalDependence::init(string **data) {
	vector<neuron> L1;
	for (int i = 0; i < dims; i++) {
		neuron ni;
		ni.components = 1 << i;
		ni.RHS = ~ni.components;
		L1.push_back(ni);
	}

	map<string, int> *temp;
	temp = new map<string, int>[dims];
	int *index = new int[dims];
	for (int i = 0; i < dims; i++) {
		index[i] = 0;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < dims; j++) {
			map<string, int>::iterator it = temp[j].find(data[i][j]);
			if (it == temp[j].end()) {
				vector<int> t;
				t.push_back(i + 1);
				L1[j].pi_set.push_back(t);
				temp[j].insert(pair<string, int>(data[i][j], index[j]++));
			}
			else {
				L1[j].pi_set[it->second].push_back(i + 1);
			}
		}
	}

	level_set.push_back(L1);
}

void FunctionalDependence::generate_next_level(int n) {

}

void FunctionalDependence::compute_dependencies(int n) {

}

void FunctionalDependence::run() {
	for (int i = 1; i < dims; i++) {
		generate_next_level(i);
		compute_dependencies(i);
	}
}