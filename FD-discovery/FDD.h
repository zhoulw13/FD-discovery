#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct neuron {
	int components; // sequence number bit format
	int RHS;

	vector<vector<int> > pi_set;

	set<neuron*> fathers;
	set<neuron*> sons;
};

struct fd{
	vector<int> LHS;
	int attr;
};

struct AttrClos {
	int combination;
	set<int> AttrSet;
};

struct Schema {
	set<int> attrs;
	vector<AttrClos> FDs;
	vector<AttrClos> attrClos;
};

class FunctionalDependence {
	int dims, size;
	vector<neuron> curr_level;
	vector<neuron> next_level;
	vector<fd> fd_set;
	vector<int> delete_set;
	int *TArray;
	vector<int> *SArray;
	vector<AttrClos> attrClosure;
	vector<AttrClos> FDClosure;
	vector<set<int>> BCNFTables;

public:
	FunctionalDependence(int, int);
	void init(string **data);
	void generate_next_level(int n);
	void compute_dependencies(int n);
	void run();
	void getAttr(int A, int B);
	void outputResult();
	void computeAttrClosure();
	void splitToBCNF();
};