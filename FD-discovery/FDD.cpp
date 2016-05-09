#include "FDD.h"

FunctionalDependence::FunctionalDependence(int n):dims(n) {
	vector<neuron> L1;
	for (int i = 0; i < dims; i++) {
		neuron ni;
		ni.components = 1 << i;
		L1.push_back(ni);
	}
	level_set.push_back(L1);
}

void FunctionalDependence::generate_next_level(int n) {
	vector<neuron> L;
	const int maxNum = pow(2, dims) - 1;
	char *flag = new char [maxNum];
	memset(flag, 0, sizeof(char) * maxNum);
	for (int i = 0; i < level_set[n].size(); i++) {
		for (int j = i + 1; j < level_set[n].size(); j++) {
			int newComponents = level_set[n][i].components & level_set[n][j].components;
			if (flag[newComponents] == 0) {
				flag[newComponents] = 1;
				neuron ns;
				// compute components
				ns.components = newComponents;
				level_set[n][i].sons.push_back(&ns);
				level_set[n][j].sons.push_back(&ns);
				ns.fathers.push_back(&(level_set[n][i]));
				ns.fathers.push_back(&(level_set[n][j]));
				// compute pi_set
				for (int l = 0; l < level_set[n][i].pi_set.size(); l++) {
					for (int m = 0; m < level_set[n][i].pi_set[l].size(); m++){

					}
				}
			}
			
		}
	}
	delete []flag;
	return;
}

void FunctionalDependence::compute_dependencies(int n) {

}

void FunctionalDependence::run() {
	for (int i = 1; i < dims; i++) {
		generate_next_level(i);
		compute_dependencies(i);
	}
}