//============================================================================
// Name        : parallel_fpgrowth.cpp
// Author      : Vitor Coimbra de Oliveira
// Version     :
// Copyright   : MIT
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include "FPTree.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cout << "Uso: " << argv[0] << " arquivo_entrada suporte_minimo num_threads"
				<< std::endl;
		exit(0);
	}
	auto file_name = std::string(argv[1]);
	int minimum_support;
	sscanf(argv[2], "%d", &minimum_support);

	int num_threads = 4;

	if (argc >= 4) {
		sscanf(argv[3], "%d", &num_threads);
	}

	FPTree tree(minimum_support, num_threads);

	auto t1 = std::chrono::high_resolution_clock::now();
	tree.build_fp_tree(file_name);
	auto things = tree.fpgrowth(std::list<int>());
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout
			<< std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
			<< std::endl;

	return 0;
}
