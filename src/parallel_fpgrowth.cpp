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
		std::cout << "Uso: " << argv[0] << " arquivo_entrada suporte_minimo"
				<< std::endl;
		exit(0);
	}
	auto file_name = std::string(argv[1]);
	int minimum_support;
	sscanf(argv[2], "%d", &minimum_support);

	FPTree tree(minimum_support);

	auto t1 = std::chrono::high_resolution_clock::now();
	tree.build_fp_tree(file_name);
	auto things = tree.fpgrowth(std::list<int>());
	auto t2 = std::chrono::high_resolution_clock::now();

	for (auto& thing : things) {
		thing.print();
		std::cout << std::endl;
	}

	std::cout
			<< std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
			<< std::endl;

	return 0;
}
