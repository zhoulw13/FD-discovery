#include "FDD.h"

FunctionalDependence::FunctionalDependence(int n):dims(n) {
	vector<neuron> L1;
	for (int i = 0; i < dims; i++) {
		neuron ni;
		ni.components.push_back(i);
		L1.push_back(ni);
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