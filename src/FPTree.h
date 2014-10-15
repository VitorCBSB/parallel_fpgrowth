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
#include <string.h>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
		assert(fp != NULL);

		count_frequent_items(fp);
		rewind(fp);
		build_tree(fp);

		fclose(fp);
	}

private:
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
		std::sort(support_vector.begin(), support_vector.end(), std::greater<Item>());
		auto new_end =
				std::remove_if(support_vector.begin(), support_vector.end(),
						[=] (const Item& item) {return item.get_count() < minimum_support;});
		support_vector.erase(new_end, support_vector.end());
	}

	std::vector<Item> extract_items(const std::string& line) {
		std::vector<Item> return_set;
		std::vector<std::string> strings;
		const char* current;
		int current_string = 0;

		for (current = &line[0]; *current != '\n' && *current != '\0'; current++) {
			if (*current == ' ') {
				current_string++;
				continue;
			}
			strings[current_string].push_back(*current);
		}

		for (const auto& string : strings) {
			return_set.push_back(Item(atoi(string.c_str())));
		}

		return return_set;
	}

	void build_tree(FILE* fp) {
		std::vector<char> line(512);
		while (fgets(&line[0], 512, fp)) {
			auto transaction_items = extract_items(std::string(&line[0]));
		}
	}
};

#endif /* FPTREE_H_ */
