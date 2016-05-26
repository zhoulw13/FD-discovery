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

	for (vector<neuron>::iterator it = L1.begin(); it != L1.end(); it++) {
		if (it->pi_set.size() == 1) {
			vector<neuron>::iterator it_temp = it;
			it_temp--;
			L1.erase(it);
			it = it_temp;
		}
	}
		
	//level_set.push_back(L1);
	curr_level = L1;

	TArray = new int[size];
	SArray = new vector<int>[size];
	memset(TArray, -1, sizeof(int) * size);

	delete []index;
	return;
}

void FunctionalDependence::generate_next_level(int n) {
	vector<neuron> L;
	vector<int> flag;
	vector<int>::iterator it;
	//int *TArray = new int [size];
	//vector<int> *SArray = new vector<int> [size];
	//memset(TArray, -1, sizeof(int) * size);
	
	int level_size = curr_level.size();
	for (int i = 0; i < level_size - 1; i++) {
		for (int j = i + 1; j < level_size; j++) {
			neuron *ni, *nj;
			ni = &curr_level[i];
			nj = &curr_level[j];
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
						if (TArray[nj->pi_set[l][m] - 1] != -1)
							SArray[TArray[nj->pi_set[l][m] - 1]].push_back(nj->pi_set[l][m]);
					}
					for (int m = 0; m < nj->pi_set[l].size(); m++) {
						if (TArray[nj->pi_set[l][m] - 1] != -1 && SArray[TArray[nj->pi_set[l][m] - 1]].size() > 0) {
							if (SArray[TArray[nj->pi_set[l][m] - 1]].size() > 1)
								ns.pi_set.push_back(SArray[TArray[nj->pi_set[l][m] - 1]]);
							SArray[TArray[nj->pi_set[l][m] - 1]].clear();
						}
					}
				}
				for (int l = 0; l < ni->pi_set.size(); l++) {
					for (int m = 0; m < ni->pi_set[l].size(); m++){
						TArray[ni->pi_set[l][m] - 1] = -1;
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
			//memset(TArray, -1, sizeof(int) * size);
		}
	}
	//level_set.push_back(L);
	next_level = L;

	//delete[] TArray;
	//delete[] SArray;
	return;
}

void FunctionalDependence::compute_dependencies(int n) {
	vector<neuron>::iterator it, temp_it;
	set<neuron*>::iterator father_it;
	int valid_tmp = (1 << dims) - 1; 
	int x_pi_length, x_minus_e_pi_length, e, father_num;
	int x_pi_sizeSum = 0, x_minus_e_pi_sizeSum = 0;

	for(int i = 0; i < next_level.size(); i++){
		int X = next_level[i].components;
		int e_set = X & next_level[i].RHS;
		int e_tag = 1;
		x_pi_length = next_level[i].pi_set.size();
		x_pi_sizeSum = 0;
		for (int k = 0; k < x_pi_length; k++) {
			x_pi_sizeSum += next_level[i].pi_set[k].size();
		}
		father_num = (next_level[i].fathers).size();
		for(int j = 0; j < dims; j++){
			e = e_set & e_tag;
			if(e != 0){
				int x_minus_e = X - e;
				for(father_it = (next_level[i].fathers).begin(); father_it != (next_level[i].fathers).end(); father_it++){
					if((*father_it)->components == x_minus_e){
						x_minus_e_pi_length = ((*father_it)->pi_set).size();
						x_minus_e_pi_sizeSum = 0;
						for (int k = 0; k < x_minus_e_pi_length; k++) {
							x_minus_e_pi_sizeSum += ((*father_it)->pi_set)[k].size();
						}
						if(x_pi_sizeSum - x_pi_length == x_minus_e_pi_sizeSum - x_minus_e_pi_length){
							getAttr(x_minus_e,e);
							next_level[i].RHS &= (~e);
							next_level[i].RHS &= X;
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
	curr_level = next_level;
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
	computeAttrClosure();
	splitToBCNF();
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

void FunctionalDependence::computeAttrClosure(){
	bool flag = false;
	set<int>::iterator it;
	pair<set<int>::iterator, bool> p; 
	int fd_set_size = fd_set.size();
	for (int i = 1; i < 1 << dims; i++) { // compute for every possible combination
		AttrClos L;
		L.combination = i;
		for (int d = 1; d <= dims; d++) {
			if ((L.combination >> (d - 1)) & 1)
				L.AttrSet.insert(d);	// insert initial attributes
		}
		int init_size = L.AttrSet.size();
		flag = true;
		while(flag) {
			flag = false;
			for (int j = 0; j < fd_set_size; j++) { // search in F over and over again until attr closure doesn't change
				int LHS_size = fd_set[j].LHS.size();
				for (int k = 0; k < LHS_size; k++) {
					it = L.AttrSet.find(fd_set[j].LHS[k]);
					if (it == L.AttrSet.end()) {
						break;
					}
					if (k == LHS_size - 1) { // all of attributes in LHS are also in the attr closure
						p = L.AttrSet.insert(fd_set[j].attr);
						if (p.second)
							flag = true;	// size of the attr closure changed
						break;
					}
				}
			}
		}
		if (L.AttrSet.size() != init_size)
			attrClosure.push_back(L);
	}
	FDClosure = attrClosure;
	// also compute FDClosure
	for (int i = 0; i < FDClosure.size(); i++) {
		for (it = FDClosure[i].AttrSet.begin(); it != FDClosure[i].AttrSet.end();) {
			if ((FDClosure[i].combination) & (1 << (*it - 1))) {
				FDClosure[i].AttrSet.erase(it++);	// erase trivial FDs
			} else
				it++;
		}
	}
}

void FunctionalDependence::splitToBCNF(){
	Schema R;
	vector<Schema> R_set;
	// insert the initial table into R_set
	R.FDs = FDClosure;
	R.attrClos = attrClosure;
	for (int i = 0; i < dims; i++)
		R.attrs.insert(i + 1);
	R_set.push_back(R);
	// find R that is not in BCNF
	for (int i = 0; i < R_set.size(); i++) {
		for (int j = 0; j < R_set[i].attrClos.size(); j++) {
			if (R_set[i].attrClos[j].AttrSet.size() != R_set[i].attrs.size()) {
				// R has a FD "alpha -> beta" that "alpha" is not a super key (i.e. R is not in BCNF)
				vector<Schema>::iterator remove_it = R_set.begin() + i;
				set<int>::iterator set_it;
				int comb = R_set[i].FDs[j].combination, index = 1;
				int beta = *(R_set[i].FDs[j].AttrSet.begin());
				Schema R1, R2;	// split R into R1 and R2 such that R1 = (alpha, beta), R2 = R - beta
				R1.attrs.insert(beta);
				while(comb != 0) {
					if (comb & 1) 
						R1.attrs.insert(index);
					comb >>= 1;
					index++;
				}
				for (set_it = R_set[i].attrs.begin(); set_it != R_set[i].attrs.end(); set_it++) {
					if (*set_it != beta)
						R2.attrs.insert(*set_it);
				}
				// arrange FDs and attr closure for two new tables
				for (int fi = 0; fi < R.FDs.size(); fi++) {
					set<int>::iterator findIt, indexIt;
					comb = R.FDs[fi].combination;
					index = 1;
					bool flag = false;
					// judge combination (i.e. LHS) for R1
					while(comb != 0) {
						if (comb & 1) {
							findIt = R1.attrs.find(index);
							if (findIt == R1.attrs.end()) {
								flag = true;
								break;
							}
						}
						comb >>= 1;
						index++;
					}
					// judge RHS for R1
					if (!flag) {
						for (indexIt = R.FDs[fi].AttrSet.begin(); indexIt != R.FDs[fi].AttrSet.end(); indexIt++) {
							findIt = R1.attrs.find(*indexIt);
							if (findIt == R1.attrs.end()) {
								flag = true;
								break;
							}
						}
					}
					if (!flag) {
						// all of LHS and RHS in this FD are contained in R1's attrs
						R1.FDs.push_back(R.FDs[fi]);
						R1.attrClos.push_back(R.attrClos[fi]);
					}

					comb = R.FDs[fi].combination;
					index = 1;
					flag = false;
					// judge combination (i.e. LHS) for R2
					while(comb != 0) {
						if (comb & 1) {
							findIt = R2.attrs.find(index);
							if (findIt == R2.attrs.end()) {
								flag = true;
								break;
							}
						}
						comb >>= 1;
						index++;
					}
					if (flag)
						continue;
					// judge RHS for R2
					for (indexIt = R.FDs[fi].AttrSet.begin(); indexIt != R.FDs[fi].AttrSet.end(); indexIt++) {
						findIt = R2.attrs.find(*indexIt);
						if (findIt == R2.attrs.end()) {
							flag = true;
							break;
						}
					}
					if (flag)
						continue;
					// all of LHS and RHS in this FD are contained in R2's attrs
					R2.FDs.push_back(R.FDs[fi]);
					R2.attrClos.push_back(R.attrClos[fi]);
				}
				// remove R from R_set and insert two new tables into R_set
				R_set.erase(remove_it);
				R_set.push_back(R1);
				R_set.push_back(R2);
				i--;
				break;
			}
		}
	}
	// print split results
	for (int i = 0; i < R_set.size(); i++) {
		for (set<int>::iterator ait = R_set[i].attrs.begin(); ait != R_set[i].attrs.end(); ait++) {
			cout << *ait << ' ';
		}
		cout << endl;
	}
}