#include <iostream>
using namespace std;

#include "Readfile.h"
#include "FDD.h"

const int dims = 12;

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "input filename as argument\n";
	}

	readfile(argv[1]);
	FunctionalDependence fdd(dims);
	fdd.run();

	return 0;
}