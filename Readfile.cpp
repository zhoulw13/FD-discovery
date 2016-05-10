#include "Readfile.h"

#include <fstream>
#include <sstream>

void readfile(const char * filename, string **data, int dims, int size) {
	ifstream in;
	in.open(filename);

	if (!in.is_open()) {
		cerr << "ERROR happened when open file\n";
	}

	string str, cmd;
	for (int i = 0; i < size; i++) {
		getline(in, str);
		stringstream s(str);
		for (int j = 0; j < dims; j++) {
			getline(s, cmd, ',');
			data[i][j] = cmd;
		}
	}
	in.close();
}