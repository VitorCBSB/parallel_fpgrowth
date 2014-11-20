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
#include <vector>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <sstream>

class FPTree {
private:
	FPTreeNodePtr root = FPTreeNodePtr(new FPTreeNode(nullptr, Item(0)));
	std::map<int, std::list<FPTreeNodePtr>> header_table;

	std::vector<Item> support_vector;
	std::map<int, Item> support_map;
	int minimum_support;

	bool single_pathed = true;

public:
	FPTree(int minimum_support) :
			minimum_support(minimum_support) {
	}

	void build_fp_tree(const std::string& file_name) {
		std::ifstream fp(file_name);
		assert(fp.is_open());

		auto transactions = read_file(fp);

		build_fp_tree(transactions);
	}

	void build_fp_tree(const std::vector<std::vector<int>>& transactions) {
		count_frequent_items(transactions);
		build_tree(transactions);
	}

	std::vector<Pattern> fpgrowth(const std::vector<int>& prefix) {
		std::vector<Pattern> result;
		if (single_pathed) {
			result = add_all_prefix_combinations(root,
					prefix);
		} else {
			result = multi_path_patterns(prefix);
		}
		return result;
	}

private:
	std::vector<std::vector<int>> read_file(std::ifstream& fp) {
		std::vector<std::vector<int>> result;
		std::string line;
		int read_item;
		while (std::getline(fp, line)) {
			std::stringstream ss(line);
			result.push_back(std::vector<int>());
			while (ss >> read_item) {
				result.back().push_back(read_item);
			}
		}
		return result;
	}

	void count_frequent_items(const std::vector<std::vector<int>>& transactions) {
		for (auto& transaction : transactions) {
			for (auto& transaction_item : transaction) {
				auto& item = support_map[transaction_item];
				item.set_value(transaction_item);
				item.increment();
			}
		}

		for (auto& item : support_map) {
			support_vector.push_back(item.second);
		}
		std::sort(support_vector.begin(), support_vector.end(),
				std::greater<Item>());
		auto new_end =
				std::remove_if(support_vector.begin(), support_vector.end(),
						[=] (const Item& item) {return item.get_count() < minimum_support;});
		support_vector.erase(new_end, support_vector.end());
	}

	void build_tree(std::vector<std::vector<int>> transactions) {
		for (auto& transaction : transactions) {
			// Remove aqueles que nao tem o suporte minimo
			auto new_end =
					std::remove_if(transaction.begin(),
							transaction.end(),
							[&](const int& item) {
								return support_map[item].get_count() < minimum_support;
							});
			transaction.erase(new_end, transaction.end());
			std::sort(transaction.begin(), transaction.end(),
					[&](const int& a, const int& b) {
						return support_map[a] > support_map[b];
					});

			add_transaction(transaction);
		}
	}

	void add_transaction(const std::vector<int>& transaction) {
		auto current_node = root;
		for (auto& item : transaction) {
			auto child = current_node->get_child(item);
			// Se nao existe ainda, cria.
			if (child == nullptr) {
				auto new_node = FPTreeNodePtr(
						new FPTreeNode(current_node, Item(item)));
				new_node->set_parent(current_node);
				current_node->add_child(new_node);

				// Cria entrada na tabela de listas
				header_table[item].push_back(new_node);

				// Setta flag de single path para falso
				// Caso a arvore tenha galhos
				if (single_pathed && !current_node->single_pathed()) {
					single_pathed = false;
				}

				// Setta o no corrente como filho
				// Para proxima iteracao
				current_node = new_node;
			} else {
				child->increment_item();
				current_node = child;
			}
		}
	}

	// TODO: fazer algoritmo de multi path
	std::vector<Pattern> multi_path_patterns(const std::vector<int>& prefix) {
		// Para cada símbolo na tabela
		for (auto it = support_vector.rbegin(); it != support_vector.rend();
				it++) {
			// Se o item nao for frequente, continua.
			if (support_map[it->get_value()].get_count() < minimum_support) {
				continue;
			}

			auto beta = std::vector<int>(prefix);
			beta.push_back(it->get_value());

			// Para cada item na lista do item que esta
			// sendo analisado agora.
			std::vector<std::vector<Item>> beta_tree_transactions;
			for (auto& item_header : header_table[it->get_value()]) {

			}

			// Construir a arvore condicional

			// Filtrar arvore condicional

			// De alguma maneira recursiva, chamar o metodo para o resto.
		}
		return std::vector<Pattern>();
	}

	std::vector<Pattern> add_all_prefix_combinations(FPTreeNodePtr current,
			const std::vector<int>& prefix) {
		std::vector<Pattern> result;
		auto itemset = std::vector<int>(prefix);
		if (current == nullptr) {
			return result;
		}
		if (current->is_root()) {
			return add_all_prefix_combinations(current->get_first_child(), prefix);
		}

		itemset.push_back(current->get_item().get_value());
		result.push_back(Pattern(itemset));

		if (current->has_children()) {
			auto temp_result = add_all_prefix_combinations(
					current->get_first_child(), itemset);
			result.insert(result.end(), temp_result.begin(), temp_result.end());
			temp_result = add_all_prefix_combinations(current->get_first_child(),
					prefix);
			result.insert(result.end(), temp_result.begin(), temp_result.end());
		}
		return result;
	}
};

#endif /* FPTREE_H_ */
