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
	vector<neuron> L;
	const int maxNum = pow(2, dims) - 1;
	char *flag = new char [maxNum];
	int *TArray = new int [size]; 
	vector<int> *SArray = new vector<int> [size];
	memset(flag, 0, sizeof(char) * maxNum);
	memset(TArray, -1, sizeof(int) * size);
	for (int i = 0; i < level_set[n].size() - 1; i++) {
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
						TArray[level_set[n][i].pi_set[l][m]] = l;
					}
				}
				for (int l = 0; l < level_set[n][j].pi_set.size(); l++) {
					for (int m = 0; m < level_set[n][j].pi_set[l].size(); m++) {
						if (TArray[level_set[n][j].pi_set[l][m]] != -1) {
							SArray[TArray[level_set[n][j].pi_set[l][m]]].push_back(level_set[n][j].pi_set[l][m]);
						}
					}
					for (int m = 0; m < level_set[n][j].pi_set[l].size(); m++) {
						if (SArray[TArray[level_set[n][j].pi_set[l][m]]].size() > 0) {
							ns.pi_set.push_back(SArray[TArray[level_set[n][j].pi_set[l][m]]]);
						}
					}
				}
				// compute RHS+
				ns.RHS = level_set[n][i].RHS & level_set[n][j].RHS;

				level_set[n + 1].push_back(ns);
			} else {
				for (int l = 0; l < level_set[n + 1].size(); l++) {
					if (level_set[n + 1][l].components == newComponents) {
						level_set[n][i].sons.push_back(&level_set[n + 1][l]);
						level_set[n][j].sons.push_back(&level_set[n + 1][l]);
						level_set[n + 1][l].fathers.push_back(&(level_set[n][i]));
						level_set[n + 1][l].fathers.push_back(&(level_set[n][j]));
						level_set[n + 1][l].RHS = level_set[n][i].RHS & level_set[n][j].RHS;
						break;
					}
				}
			}			
		}
	}
	delete []flag;
	delete []TArray;
	delete []SArray;
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