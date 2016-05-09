#include <iostream>
#include <vector>
using namespace std;

struct neuron {
	int components; // sequence number 0-11
	vector<int> RHS;
	vector<vector<int>> pi_set;

	vector<neuron*> fathers;
	vector<neuron*> sons;
};

class FunctionalDependence {
	int dims;
	vector<vector<neuron>> level_set;

public:
	FunctionalDependence(int);
	void generate_next_level(int n);
	void compute_dependencies(int n);
	void run();
};