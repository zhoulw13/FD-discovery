#include <time.h>
#include <iostream>
using namespace std;

#include "Readfile.h"
#include "FDD.h"

const int dims = 12;
const int size = 14970;


string **init() {
	string **data = new string*[size];
	for (int i = 0; i < size; i++) {
		data[i] = new string[dims];
	}
	return data;
}

void release(string **data) {
	for (int i = 0; i < size; i++) {
		delete[]data[i];
	}
	
	delete[]data;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "input filename as argument\n";
	}

	clock_t start = clock();

	string **data = init();
	readfile(argv[1], data, dims, size);
	FunctionalDependence fdd(dims, size);
	fdd.init(data);
	//fdd.run();
	release(data);

	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << time << endl;

	return 0;
}