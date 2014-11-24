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
#include "Pattern.h"
#include <iostream>
#include <algorithm>
#include <string.h>
#include <memory>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <omp.h>

class FPTree {
private:
	FPTreeNodePtr root = FPTreeNodePtr(new FPTreeNode(nullptr, Item(0)));
	std::map<int, std::list<FPTreeNodePtr>> header_table;

	std::vector<Item> support_list;
	std::map<int, Item> support_map;
	int minimum_support;
	int num_threads = 4;

	bool single_pathed = true;

	static bool launch_threads;

public:
	FPTree(int minimum_support, int num_threads = 4) :
			minimum_support(minimum_support), num_threads(num_threads) {
	}

	void build_fp_tree(const std::string& file_name);

	void build_fp_tree(const std::list<std::list<int>>& transactions);

	std::list<Pattern> fpgrowth(const std::list<int>& prefix);

	void print() {
		root->print();
	}

private:
	FPTree(int minimum_support, const std::list<std::list<Item>>& prefix_paths);

	void build_from_prefix_paths(
			const std::list<std::list<Item>>& prefix_paths);

	void build_tree_from_prefix_paths(
			const std::list<std::list<Item>>& prefix_paths);

	void build_support_list(const std::map<int, Item>& support_map);

	std::list<std::list<int>> read_file(std::ifstream& fp);

	void count_frequent_items(const std::list<std::list<int>>& transactions);

	void build_tree(const std::list<std::list<int>>& transactions);

	void add_transaction(const std::list<int>& transaction);

	std::list<Pattern> multi_path_patterns(const std::list<int>& prefix);

	std::list<Pattern> add_all_prefix_combinations(FPTreeNodePtr current,
			const std::list<int>& prefix);

	void build_support_from_prefix_paths(
			const std::list<std::list<Item> >& prefix_paths);

	bool is_null() {
		return !root->has_children();
	}
};

#endif /* FPTREE_H_ */
