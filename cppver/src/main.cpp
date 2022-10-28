#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "notc.h"

int main(int argc, char **argv) {

	if (argc == 1) {
		Notc::run_prompt();
	} else if (argc == 2) {
		Notc::run_from_file(argv[1]);	
	} else {
		std::cout << "Usage: notc [notc_file]\n";
		return -1;
	}

	
	return 0;
}
