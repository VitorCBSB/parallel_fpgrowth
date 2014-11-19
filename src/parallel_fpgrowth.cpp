//============================================================================
// Name        : parallel_fpgrowth.cpp
// Author      : Vitor Coimbra de Oliveira
// Version     :
// Copyright   : MIT
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string>
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

	tree.build_fp_tree(file_name);

	return 0;
}
