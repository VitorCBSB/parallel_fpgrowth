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
#include "Item.h"

class FPTreeNode {
private:
	Item item;
	std::shared_ptr<FPTreeNode> parent;
	std::vector<std::shared_ptr<FPTreeNode>> children;

public:
	FPTreeNode(std::shared_ptr<FPTreeNode> parent, Item item) :
			parent(parent), item(item) {
	}
};

typedef std::shared_ptr<FPTreeNode> FPTreeNodePtr;

#endif /* FPTREENODE_H_ */
