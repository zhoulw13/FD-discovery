#include <iostream>
#include <vector>
using namespace std;

struct neuron {
	vector<int> components; // sequence number 0-11
	vector<int> RHS;
	vector<string> pi_set;

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