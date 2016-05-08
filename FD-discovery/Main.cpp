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

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "input filename as argument\n";
	}

	string **data = init();
	readfile(argv[1], data, dims, size);
	FunctionalDependence fdd(dims, size);
	fdd.init(data);
	fdd.run();

	return 0;
}