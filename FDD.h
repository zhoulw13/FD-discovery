#include <iostream>
#include <vector>
using namespace std;

struct neuron {
	int components; // sequence number bit format
	int RHS;

	vector< vector<int> > pi_set;

	vector<neuron*> fathers;
	vector<neuron*> sons;
};

struct fd{
	vector<int> LHS;
	int attr;
};

class FunctionalDependence {
	int dims, size;
	vector< vector<neuron> > level_set;
	vector<fd> fd_set;

public:
	FunctionalDependence(int, int);
	void init(string **data);
	void generate_next_level(int n);
	void compute_dependencies(int n);
	void run();
	void getAttr(int A, int B);
	void outputResult();
};
