#include <iostream>
#include <vector>
using namespace std;

struct neuron {
	int components; // sequence number bit format
	int RHS;

	vector<vector<int>> pi_set;

	vector<neuron*> fathers;
	vector<neuron*> sons;
};

class FunctionalDependence {
	int dims, size;
	vector<vector<neuron>> level_set;

public:
	FunctionalDependence(int, int);
	void init(string **data);
	void generate_next_level(int n);
	void compute_dependencies(int n);
	void run();
};