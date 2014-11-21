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
			result = add_all_prefix_combinations(root, prefix);
		} else {
			result = multi_path_patterns(prefix);
		}
		return result;
	}

private:
	FPTree(int minimum_support,
			const std::vector<std::vector<Item>>& prefix_paths) :
			minimum_support(minimum_support) {
		build_from_prefix_paths(prefix_paths);
	}

	void build_from_prefix_paths(
			const std::vector<std::vector<Item>>& prefix_paths) {
		build_support_from_prefix_paths(prefix_paths);
		build_tree_from_prefix_paths(prefix_paths);
	}

	void build_tree_from_prefix_paths(const std::vector<std::vector<Item>>& prefix_paths) {
		for (auto& prefix_path : prefix_paths) {
			auto& current_node = root;
			auto count = prefix_path[0].count;
			// Itera em ordem contraria,
			// Ignorando o primeiro.
			for (auto it = prefix_path.end() - 1; it != prefix_path.begin(); it++) {
				// Pula items infrequentes.
				if (support_map[it->value].count < minimum_support) {
					continue;
				}

				auto child = current_node->get_child(it->value);
				if (child == nullptr) {
					auto new_node = FPTreeNodePtr(
							new FPTreeNode(current_node, *it));
					new_node->get_item().count = count; // Atualiza suporte.
					current_node->add_child(new_node);

					// Cria entrada na tabela de listas
					header_table[it->value].push_back(new_node);

					// Setta flag de single path para falso
					// Caso a arvore tenha galhos
					if (single_pathed && !current_node->single_pathed()) {
						single_pathed = false;
					}

					// Setta o no corrente como filho
					// Para proxima iteracao
					current_node = new_node;
				} else {
					child->get_item().count += count;
					current_node = child;
				}
			}
		}
	}

	void build_support_vector(const std::map<int, Item>& support_map) {
		for (auto& item : support_map) {
			support_vector.push_back(item.second);
		}
		std::sort(support_vector.begin(), support_vector.end(),
				std::greater<Item>());
		auto new_end =
				std::remove_if(support_vector.begin(), support_vector.end(),
						[=] (const Item& item) {return item.count < minimum_support;});
		support_vector.erase(new_end, support_vector.end());
	}

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

	void count_frequent_items(
			const std::vector<std::vector<int>>& transactions) {
		for (auto& transaction : transactions) {
			for (auto& transaction_item : transaction) {
				auto& item = support_map[transaction_item];
				item.set_value(transaction_item);
				item.increment();
			}
		}

		build_support_vector(support_map);
	}

	void build_tree(std::vector<std::vector<int>> transactions) {
		for (auto& transaction : transactions) {
			// Remove aqueles que nao tem o suporte minimo
			auto new_end = std::remove_if(transaction.begin(),
					transaction.end(), [&](const int& item) {
						return support_map[item].count < minimum_support;
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
		std::vector<Pattern> result;
		// Para cada símbolo na tabela
		for (auto it = support_vector.rbegin(); it != support_vector.rend();
				it++) {
			// Se o item nao for frequente, continua.
			if (support_map[it->value].count < minimum_support) {
				continue;
			}

			auto beta = std::vector<int>(prefix);
			beta.push_back(it->value);
			auto pattern = Pattern(beta, support_map[it->value].count);
			result.push_back(pattern);

			// Para cada item na lista do item que esta
			// sendo analisado agora.
			// Constroi uma lista de transacoes da arvore Beta.
			//
			// Atenção: o item base e colocado na lista tambem,
			// mas ele nao deve estar la. Ele e ignorado na cons-
			// trucao da nova FPTree Beta, so e usado para pegar
			// seu count.
			std::vector<std::vector<Item>> beta_tree_transactions;
			for (auto& item_header : header_table[it->value]) {
				auto& current = item_header;
				beta_tree_transactions.push_back(std::vector<Item>());
				while (!current->is_root()) {
					beta_tree_transactions.back().push_back(
							current->get_item());
				}
			}

			FPTree beta_tree(minimum_support, beta_tree_transactions);

			if (!beta_tree.is_null()) {
				auto temp_result = beta_tree.fpgrowth(beta);
				result.insert(result.end(), temp_result.begin(), temp_result.end());
			}
		}
		return result;
	}

	std::vector<Pattern> add_all_prefix_combinations(FPTreeNodePtr current,
			const std::vector<int>& prefix) {
		std::vector<Pattern> result;
		auto itemset = std::vector<int>(prefix);
		if (current == nullptr) {
			return result;
		}
		if (current->is_root()) {
			return add_all_prefix_combinations(current->get_first_child(),
					prefix);
		}

		itemset.push_back(current->get_item().value);
		result.push_back(Pattern(itemset, current->get_item().count));

		if (current->has_children()) {
			auto temp_result = add_all_prefix_combinations(
					current->get_first_child(), itemset);
			result.insert(result.end(), temp_result.begin(), temp_result.end());
			temp_result = add_all_prefix_combinations(
					current->get_first_child(), prefix);
			result.insert(result.end(), temp_result.begin(), temp_result.end());
		}
		return result;
	}

	void build_support_from_prefix_paths(
			const std::vector<std::vector<Item> >& prefix_paths) {
		for (auto& prefix_path : prefix_paths) {
			auto count = prefix_path[0].count;
			// Pula o primeiro porque e o item que esta
			// Sendo analisado, portanto nao interessa.
			for (auto it = prefix_path.begin() + 1; it != prefix_path.end();
					it++) {
				// Ja existe no mapa?
				if (support_map.find(it->value) == support_map.end()) {
					support_map[it->value] = Item(it->value);
					support_map[it->value].count = count;
				} else {
					support_map[it->value].count += count;
				}
			}
		}
		build_support_vector(support_map);
	}

	bool is_null() {
		return !root->has_children();
	}
};

#endif /* FPTREE_H_ */
