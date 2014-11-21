/*
 * Pattern.h
 *
 *  Created on: 18/11/2014
 *      Author: vitor
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include <list>
#include <iostream>
#include <string>
#include "Item.h"

class Pattern {
private:
	std::list<int> pattern_set;
	int count;

public:
	Pattern(std::list<int> pattern_set, int count) :
			pattern_set(pattern_set), count(count) {
	}

	void print() {
		std::cout << '[';
		for (auto it = pattern_set.begin(); it != pattern_set.end(); it++) {
			std::cout << *it;
			auto next_item = it;
			next_item++;
			if (next_item == pattern_set.end()) {
				std::cout << "";
			} else {
				std::cout << ", ";
			}
		}
		std::cout << "] : " << count;
	}
};

#endif /* PATTERN_H_ */
