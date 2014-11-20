/*
 * Pattern.h
 *
 *  Created on: 18/11/2014
 *      Author: vitor
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include <vector>
#include <iostream>
#include <string>
#include "Item.h"

class Pattern {
private:
	std::vector<int> pattern_set;

public:
	Pattern(std::vector<int> pattern_set) :
			pattern_set(pattern_set) {
	}

	void print() {
		std::cout << '[';
		for (auto it = pattern_set.begin(); it != pattern_set.end(); it++) {
			std::cout << *it;
			if ((it + 1) == pattern_set.end()) {
				std::cout << "";
			} else {
				std::cout << ", ";
			}
		}
		std::cout << ']';
	}
};

#endif /* PATTERN_H_ */
