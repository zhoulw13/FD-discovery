#include "FDD.h"
#include <algorithm>

FunctionalDependence::FunctionalDependence(int n, int s):dims(n), size(s) {
	vector<neuron> L1;
	for (int i = 0; i < dims; i++) {
		neuron ni;
		ni.components = 1 << i;
		L1.push_back(ni);
	}
	level_set.push_back(L1);
}

void FunctionalDependence::init(string **data) {
	vector<string> *temp;
	temp = new vector<string>[dims];

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < dims; j++) {
			int pos = find(temp[j].begin(), temp[j].end(), data[i][j]) - temp[j].begin();
			if (pos < temp[j].size()) {
				level_set[0][j].pi_set[pos].push_back(i+1);
			}
			else {
				vector<int> t;
				t.push_back(i+1);
				level_set[0][j].pi_set.push_back(t);
				temp[j].push_back(data[i][j]);
			}
		}
	}

	cout << size << " " << int(temp[0].end() - temp[0].begin()) << '\n';
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