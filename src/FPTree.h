/*
 * FPTree.h
 *
 *  Created on: 14/10/2014
 *      Author: vitor
 */

#ifndef FPTREE_H_
#define FPTREE_H_

#include "FPTreeNode.h"
#include "Item.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <stdio.h>

class FPTree {
private:
	FPTreeNodePtr root = FPTreeNodePtr(new FPTreeNode(nullptr, Item(0)));
	std::map<int, std::list<FPTreeNodePtr>> quick_reference_list;

	std::vector<Item> support_vector;
	int minimum_support;

public:
	FPTree(int minimum_support) :
			minimum_support(minimum_support) {
	}

	void build_fp_tree(const std::string& file_name) {
		FILE* fp = fopen(file_name.c_str(), "r");
		if (!fp) {
			std::cerr << "Nao consegui abrir o arquivo: " << file_name
					<< std::endl;
			exit(1);
		}

		count_frequent_items(fp);
		rewind(fp);
		build_tree(fp);

		fclose(fp);
	}

	void count_frequent_items(FILE* fp) {
		std::map<int, Item> support_map;
		int read_item;

		while (fscanf(fp, "%d", &read_item) != EOF) {
			auto& item = support_map[read_item];
			item.set_value(read_item);
			item.increment();
		}

		for (auto& item : support_map) {
			support_vector.push_back(item.second);
		}
		std::sort(support_vector.begin(), support_vector.end());
		std::reverse(support_vector.begin(), support_vector.end());
		auto new_end =
				std::remove_if(support_vector.begin(), support_vector.end(),
						[=] (const Item& item) {return item.get_count() < minimum_support;});
		support_vector.erase(new_end, support_vector.end());
	}

	void build_tree(FILE* fp) {

	}
};

#endif /* FPTREE_H_ */
