/*
 * Item.h
 *
 *  Created on: 14/10/2014
 *      Author: vitor
 */

#ifndef ITEM_H_
#define ITEM_H_

#include <iostream>

class Item {
public:
	int count = 1;
	int value;

	Item() :
			count(0), value(0) {
	}

	Item(int value) :
			value(value) {
	}

	int get_value() const {
		return value;
	}

	void set_value(int value) {
		this->value = value;
	}

	int get_count() const {
		return count;
	}

	void increment() {
		count++;
	}

	bool operator<(const Item& outro) const {
		return count < outro.count;
	}

	bool operator>(const Item& outro) const {
		return count > outro.count;
	}

	void print() {
		std::cout << '(' << value << ", " << count << ')';
	}
};

#endif /* ITEM_H_ */
