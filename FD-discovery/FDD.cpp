#include "FDD.h"
#include <algorithm>
#include <string>
#include <string.h>
#include <math.h>
#include <fstream>
#include <map>
#include <bitset>

FunctionalDependence::FunctionalDependence(int n, int s):dims(n), size(s) {
}

void FunctionalDependence::init(string **data) {
	vector<neuron> L1;
	for (int i = 0; i < dims; i++) {
		neuron ni;
		ni.components = 1 << i;
		ni.RHS = (1 << dims) - 1;
		L1.push_back(ni);
	}

	map<string, int> *temp;
	temp = new map<string, int>[dims];
	int *index = new int[dims];
	memset(index, 0, sizeof(int) * dims);

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

	delete []index;
	return;
}

void FunctionalDependence::generate_next_level(int n) {
	vector<neuron> L;
	vector<int> flag;
	vector<int>::iterator it;

	const int maxNum = 1 << dims - 1;
	int *TArray = new int [size];
	vector<int> *SArray = new vector<int> [size];
	memset(TArray, -1, sizeof(int) * size);
	
	int level_size = level_set[n].size();
	for (int i = 0; i < level_size - 1; i++) {
		for (int j = i + 1; j < level_size; j++) {
			neuron *ni, *nj;
			ni = &level_set[n][i];
			nj = &level_set[n][j];
			int newComponents = ni->components | nj->components;
			int count = newComponents & 1, temp = newComponents;
			for (int k = 0; k < dims; k++) {
				temp >>= 1;
				count += temp & 1;
			}
			//check validation
			if (count != n + 2)
				continue;

			it = find(flag.begin(), flag.end(), newComponents);
			if (it == flag.end()) {
				flag.push_back(newComponents);
				neuron ns;
				// compute components

				ns.components = newComponents;
				ni->sons.insert(&ns);
				nj->sons.insert(&ns);
				ns.fathers.insert(ni);
				ns.fathers.insert(nj);
				// compute pi_set
				for (int l = 0; l < ni->pi_set.size(); l++) {
					for (int m = 0; m < ni->pi_set[l].size(); m++){
						TArray[ni->pi_set[l][m] - 1] = l;
					}
				}
				for (int l = 0; l < nj->pi_set.size(); l++) {
					for (int m = 0; m < nj->pi_set[l].size(); m++) {
						SArray[TArray[nj->pi_set[l][m] - 1]].push_back(nj->pi_set[l][m]);
					}
					for (int m = 0; m < nj->pi_set[l].size(); m++) {
						if (SArray[TArray[nj->pi_set[l][m] - 1]].size() > 0) {
							ns.pi_set.push_back(SArray[TArray[nj->pi_set[l][m] - 1]]);
							SArray[TArray[nj->pi_set[l][m] - 1]].clear();
						}
					}
				}
				// compute RHS+
				ns.RHS = ni->RHS & nj->RHS;
				L.push_back(ns);
			} else {
				int l = it - flag.begin();
				ni->sons.insert(&L[l]);
				nj->sons.insert(&L[l]);
				L[l].fathers.insert(ni);
				L[l].fathers.insert(nj);
				L[l].RHS = ni->RHS & nj->RHS & L[l].RHS;
			}		
			memset(TArray, -1, sizeof(int) * size);
		}
	}
	level_set.push_back(L);

	delete[] TArray;
	delete[] SArray;
	return;
}

void FunctionalDependence::compute_dependencies(int n) {
	vector<neuron>::iterator it, temp_it;
	set<neuron*>::iterator father_it;
	int valid_tmp = (1 << dims) - 1; 
	int x_pi_length, x_minus_e_pi_length, e, father_num;
	for(it=level_set[n].begin();it!=level_set[n].end();it++){
		int X = it->components;
		int e_set = X & it->RHS;
		int e_tag = 1;
		x_pi_length = it->pi_set.size();
		father_num = (it->fathers).size();
		for(int i=0;i<dims;i++){
			e = e_set & e_tag;
			if(e != 0){
				int x_minus_e = X - e;
				for(father_it = (it->fathers).begin();father_it != (it->fathers).end();father_it++){
					if((*father_it)->components == x_minus_e){
						x_minus_e_pi_length = ((*father_it)->pi_set).size();
						if(x_pi_length == x_minus_e_pi_length){
							getAttr(x_minus_e,e);
							it->RHS = it->RHS & (~e);
							it->RHS = it-> RHS & X;
						}
						break;
					}
				}
			}
			e_tag = e_tag << 1;
		}
	}
}

bool SortByM1( const fd &v1, const fd &v2)
{  
	int len1 = v1.LHS.size(), len2 = v2.LHS.size();
	for(int i=0;i<len1 && i<len2;i++){
		if(v1.LHS.at(i) < v2.LHS.at(i))
			return true;
		else if(v1.LHS.at(i) > v2.LHS.at(i))
			return false;
		if(i == len1 - 1 && i < len2 - 1)
			return true;
		else if(i == len2 - 1 && i < len1 - 1)
			return false;
	}
	return (v1.attr < v2.attr);
}  

void FunctionalDependence::run() {
	for (int i = 1; i < dims; i++) {
		generate_next_level(i-1);
		compute_dependencies(i);
	}
	sort(fd_set.begin(),fd_set.end(),SortByM1);  
	outputResult();
}

void FunctionalDependence::getAttr(int A, int  B){
	struct fd fd_temp;
	int i, e_tag = 1;
	for(i=1;i<=dims;i++){
		if((A & e_tag) != 0){
			fd_temp.LHS.push_back(i);
		}
		e_tag = e_tag << 1;
	}
	e_tag = 1;
	for(i=1;i<=dims;i++){
		if((B & e_tag) != 0){
			fd_temp.attr = i;
			break;
		}
		e_tag = e_tag << 1;
	}
	fd_set.push_back(fd_temp);
}

void FunctionalDependence::outputResult(){
	ofstream fout("output.txt");
	int len, i;
	vector<fd>::iterator it;
	for(it=fd_set.begin();it!=fd_set.end();it++){
		len = it->LHS.size();
		for(i=0;i<len-1;i++){
			fout<<it->LHS.at(i)<<" ";
		}
		fout<<it->LHS.at(i)<<" -> ";
		fout<<it->attr<<endl;
	}
}