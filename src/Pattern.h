/*
 * Pattern.h
 *
 *  Created on: 18/11/2014
 *      Author: vitor
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include <vector>
#include "Item.h"

class Pattern {
private:
	std::vector<Item> pattern_set;

public:
	void add_item(Item item) {
		pattern_set.push_back(item);
	}
};

#endif /* PATTERN_H_ */
