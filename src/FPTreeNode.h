/*
 * FPTreeNode.h
 *
 *  Created on: 14/10/2014
 *      Author: vitor
 */

#ifndef FPTREENODE_H_
#define FPTREENODE_H_

#include <memory>
#include <vector>
#include <map>
#include "Item.h"

class FPTreeNode {
private:
	Item item;
	std::shared_ptr<FPTreeNode> parent;
	std::map<int, std::shared_ptr<FPTreeNode>> children;

public:
	FPTreeNode(std::shared_ptr<FPTreeNode> parent, Item item) :
			item(item), parent(parent) {
	}

	void increment_item() {
		item.increment();
	}

	void add_child(std::shared_ptr<FPTreeNode> child) {
		children[child->item.value] = child;
	}

	void set_parent(std::shared_ptr<FPTreeNode> parent) {
		this->parent = parent;
	}

	std::shared_ptr<FPTreeNode> get_parent() {
		return parent;
	}

	bool single_pathed() {
		return children.size() <= 1;
	}

	bool has_children() {
		return !children.empty();
	}

	bool is_root() {
		return parent == nullptr;
	}

	Item& get_item() {
		return item;
	}

	std::shared_ptr<FPTreeNode> get_first_child() {
		if (children.empty()) {
			return nullptr;
		}
		return (*children.begin()).second;
	}

	std::shared_ptr<FPTreeNode> get_child(int value) {
		std::map<int, std::shared_ptr<FPTreeNode>>::iterator child_location =
				children.find(value);
		if (child_location == children.end()) {
			return nullptr;
		}
		return (*child_location).second;
	}

	void print() {
		if (!is_root()) {
			std::cout << item.value << '(' << item.count << ')' << ' ';
		}

		for (auto& child : children) {
			child.second->print();
		}
		std::cout << std::endl;
	}

};

typedef std::shared_ptr<FPTreeNode> FPTreeNodePtr;

#endif /* FPTREENODE_H_ */
