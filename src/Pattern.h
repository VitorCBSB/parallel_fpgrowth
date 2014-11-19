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
	std::vector<int> pattern_set;
	int count = 1;

public:
	Pattern(std::vector<int> pattern_set) :
			pattern_set(pattern_set) {
	}

	void increment() {
		count++;
	}
};

#endif /* PATTERN_H_ */
