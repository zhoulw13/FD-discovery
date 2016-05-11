#include "FDD.h"
#include <algorithm>
#include <string>
#include <string.h>
#include <math.h>
#include <fstream>
<<<<<<< HEAD
=======
#include <set>
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
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
<<<<<<< HEAD
	vector<int>::iterator delete_it;
=======
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
	const int maxNum = 1 << dims - 1;
	set<int> flag;
	set<int>::iterator setIt;
	int *TArray = new int [size];
	vector<int> *SArray = new vector<int> [size];
	memset(TArray, -1, sizeof(int) * size);
	//cout<<delete_set.size()<<endl;
	for (int i = 0; i < level_set[n].size() - 1; i++) {
		for (int j = i + 1; j < level_set[n].size(); j++) {
			int newComponents = level_set[n][i].components | level_set[n][j].components;
			int count = newComponents & 1, temp = newComponents;
<<<<<<< HEAD
			for (int k = 0; k < dims; k++) {
				temp >>= 1;
				count += temp & 1;
			}
			//check validation
			if (count != n + 2)
				continue;

			/*for (delete_it = delete_set.begin(); delete_it != delete_set.end(); delete_it++) {
				if (((*delete_it) & newComponents) == (*delete_it)) {
					break;
				}
			}
			if (delete_it != delete_set.end()) {
				continue;
			}
			*/
=======
			for (int k = 0; k < 12; k++) {
				temp >>= 1;
				count += temp & 1;
			}
			if (count != n + 2)
				continue;
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
			//cout << bitset<32>(newComponents) << endl;
			setIt = flag.find(newComponents);
			if (setIt == flag.end()) {
				flag.insert(newComponents);
				neuron ns;
				// compute components
				ns.components = newComponents;
				level_set[n][i].sons.insert(&ns);
				level_set[n][j].sons.insert(&ns);
				ns.fathers.insert(&(level_set[n][i]));
				ns.fathers.insert(&(level_set[n][j]));
				// compute pi_set
				for (int l = 0; l < level_set[n][i].pi_set.size(); l++) {
					for (int m = 0; m < level_set[n][i].pi_set[l].size(); m++){
						TArray[level_set[n][i].pi_set[l][m] - 1] = l;
					}
				}
				for (int l = 0; l < level_set[n][j].pi_set.size(); l++) {
					for (int m = 0; m < level_set[n][j].pi_set[l].size(); m++) {
						SArray[TArray[level_set[n][j].pi_set[l][m] - 1]].push_back(level_set[n][j].pi_set[l][m]);
					}
					for (int m = 0; m < level_set[n][j].pi_set[l].size(); m++) {
						if (SArray[TArray[level_set[n][j].pi_set[l][m] - 1]].size() > 0) {
							ns.pi_set.push_back(SArray[TArray[level_set[n][j].pi_set[l][m] - 1]]);
							SArray[TArray[level_set[n][j].pi_set[l][m] - 1]].clear();
						}
					}
				}
				// compute RHS+
				ns.RHS = level_set[n][i].RHS & level_set[n][j].RHS;
				L.push_back(ns);
			} else {
				for (int l = 0; l < L.size(); l++) {
					if (L[l].components == newComponents) {
<<<<<<< HEAD
						level_set[n][i].sons.insert(&L[l]);
						level_set[n][j].sons.insert(&L[l]);
						L[l].fathers.insert(&(level_set[n][i]));
						L[l].fathers.insert(&(level_set[n][j]));
						L[l].RHS = level_set[n][i].RHS & level_set[n][j].RHS & L[l].RHS;
=======
						level_set[n][i].sons.push_back(&L[l]);
						level_set[n][j].sons.push_back(&L[l]);
						L[l].fathers.push_back(&(level_set[n][i]));
						L[l].fathers.push_back(&(level_set[n][j]));
						L[l].RHS = level_set[n][i].RHS & level_set[n][j].RHS;
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
						break;
					}
				}
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
<<<<<<< HEAD
		x_pi_length = it->pi_set.size();
		father_num = (it->fathers).size();
=======
		x_pi_length = it->pi_set.size(); 
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
		for(int i=0;i<dims;i++){
			e = e_set & e_tag;
			if(e != 0){
				int x_minus_e = X - e;
<<<<<<< HEAD
				for(father_it = (it->fathers).begin();father_it != (it->fathers).end();father_it++){
					if((*father_it)->components == x_minus_e){
						x_minus_e_pi_length = ((*father_it)->pi_set).size();
=======
				int father_num = (it->fathers).size();
				for(int j=0;j<father_num;j++){
					if((it->fathers.at(j))->components == x_minus_e){
						x_minus_e_pi_length = ((it->fathers.at(j))->pi_set).size();
>>>>>>> 6a2164da0ed4b7707b0410b6d5d8179d2655ec8a
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
		/*if(it->RHS == 0){
			delete_set.push_back(it->components);
			temp_it = it;
			temp_it--;
			level_set[n].erase(it);
			it = temp_it;
		}*/
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