/*
 * FPTree.cpp
 *
 *  Created on: 21/11/2014
 *      Author: vitor
 */

#include "FPTree.h"

bool FPTree::launch_threads = true;

FPTree::FPTree(int minimum_support,
		const std::list<std::list<Item>>& prefix_paths) :
		minimum_support(minimum_support) {
	build_from_prefix_paths(prefix_paths);
}

void FPTree::build_from_prefix_paths(
		const std::list<std::list<Item>>& prefix_paths) {
	build_support_from_prefix_paths(prefix_paths);
	build_tree_from_prefix_paths(prefix_paths);
}

void FPTree::build_tree_from_prefix_paths(
		const std::list<std::list<Item>>& prefix_paths) {
	for (auto& prefix_path : prefix_paths) {
		auto current_node = root;
		auto count = prefix_path.front().count;
		// Itera em ordem contraria,
		// Ignorando o primeiro.
		auto path_end = prefix_path.rend();
		path_end--;
		for (auto it = prefix_path.rbegin(); it != path_end; it++) {
			// Pula items infrequentes.
			if (support_map.find(it->value) == support_map.end()
					|| support_map[it->value].count < minimum_support) {
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

void FPTree::build_support_list(const std::map<int, Item>& support_map) {
	for (auto& item : support_map) {
		support_list.push_back(item.second);
	}
	std::sort(support_list.begin(), support_list.end(), std::greater<Item>());
	auto new_end = std::remove_if(support_list.begin(), support_list.end(),
			[&] (const Item& item) {return item.count < minimum_support;});
	support_list.erase(new_end, support_list.end());
}

std::list<std::list<int>> FPTree::read_file(std::ifstream& fp) {
	std::list<std::list<int>> result;
	std::string line;
	int read_item;
	while (std::getline(fp, line)) {
		std::stringstream ss(line);
		result.push_back(std::list<int>());
		while (ss >> read_item) {
			result.back().push_back(read_item);
		}
	}
	return result;
}

void FPTree::count_frequent_items(
		const std::list<std::list<int>>& transactions) {
	for (auto& transaction : transactions) {
		for (auto& transaction_item : transaction) {
			auto& item = support_map[transaction_item];
			item.set_value(transaction_item);
			item.increment();
		}
	}

	build_support_list(support_map);
}

void FPTree::build_tree(const std::list<std::list<int>>& transactions) {
	for (auto& transaction : transactions) {
		auto transaction_copy = std::list<int>(transaction);
		// Remove aqueles que nao tem o suporte minimo
		auto new_end = std::remove_if(transaction_copy.begin(),
				transaction_copy.end(), [&](const int& item) {
					return support_map[item].count < minimum_support;
				});
		transaction_copy.erase(new_end, transaction_copy.end());
		transaction_copy.sort([&](const int& a, const int& b) {
			return support_map[a] > support_map[b];
		});

		add_transaction(transaction_copy);
	}
}

void FPTree::add_transaction(const std::list<int>& transaction) {
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

std::list<Pattern> FPTree::multi_path_patterns(const std::list<int>& prefix) {
	std::list<Pattern> result;
	// Para cada símbolo na tabela
#pragma omp parallel for if(launch_threads) num_threads(num_threads)
	for (auto it = support_list.rbegin(); it < support_list.rend(); it++) {
		// Setta a flag de disparar threads para falso.
		// So queremos disparar threads na primeira execucao.
		launch_threads = false;

		// Se o item nao for frequente, continua.
		if (support_map[it->value].count < minimum_support) {
			continue;
		}

		auto beta = std::list<int>(prefix);
		beta.push_front(it->value);
		auto pattern = Pattern(beta, support_map[it->value].count);
#pragma omp critical
		result.push_back(pattern);

		// Para cada item na lista do item que esta
		// sendo analisado agora.
		// Constroi uma lista de transacoes da arvore Beta.
		//
		// Atenção: o item base e colocado na lista tambem,
		// mas ele nao deve estar la. Ele e ignorado na cons-
		// trucao da nova FPTree Beta, so e usado para pegar
		// seu count.
		std::list<std::list<Item>> beta_tree_prefix_paths;
		for (auto& item_header : header_table[it->value]) {
			auto current = item_header;
			beta_tree_prefix_paths.push_back(std::list<Item>());
			while (!current->is_root()) {
				beta_tree_prefix_paths.back().push_back(current->get_item());
				current = current->get_parent();
			}
		}

		FPTree beta_tree(minimum_support, beta_tree_prefix_paths);

		if (!beta_tree.is_null()) {
			auto temp_result = beta_tree.fpgrowth(beta);
#pragma omp critical
			result.insert(result.end(), temp_result.begin(), temp_result.end());
		}
	}
	return result;
}

std::list<Pattern> FPTree::add_all_prefix_combinations(FPTreeNodePtr current,
		const std::list<int>& prefix) {
	std::list<Pattern> result;
	auto itemset = std::list<int>(prefix);
	if (current == nullptr) {
		return result;
	}
	if (current->is_root()) {
		return add_all_prefix_combinations(current->get_first_child(), prefix);
	}

	itemset.push_front(current->get_item().value);
	result.push_back(Pattern(itemset, current->get_item().count));

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

void FPTree::build_support_from_prefix_paths(
		const std::list<std::list<Item> >& prefix_paths) {
	for (auto& prefix_path : prefix_paths) {
		auto count = prefix_path.front().count;
		// Pula o primeiro porque e o item que esta
		// Sendo analisado, portanto nao interessa.
		auto list_begin = prefix_path.begin();
		list_begin++;
		for (auto it = list_begin; it != prefix_path.end(); it++) {
			// Ja existe no mapa?
			if (support_map.find(it->value) == support_map.end()) {
				support_map[it->value] = Item(it->value);
				support_map[it->value].count = count;
			} else {
				support_map[it->value].count += count;
			}
		}
	}
	build_support_list(support_map);
}

void FPTree::build_fp_tree(const std::string& file_name) {
	std::ifstream fp(file_name);
	assert(fp.is_open());

	auto transactions = read_file(fp);

	build_fp_tree(transactions);
}

void FPTree::build_fp_tree(const std::list<std::list<int>>& transactions) {
	count_frequent_items(transactions);
	build_tree(transactions);
}

std::list<Pattern> FPTree::fpgrowth(const std::list<int>& prefix) {
	std::list<Pattern> result;
	if (single_pathed) {
		result = add_all_prefix_combinations(root, prefix);
	} else {
		result = multi_path_patterns(prefix);
	}
	return result;
}
