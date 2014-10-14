/*
 * Item.h
 *
 *  Created on: 14/10/2014
 *      Author: vitor
 */

#ifndef ITEM_H_
#define ITEM_H_

class Item {
private:
	int count = 1;
	int value;

public:
	Item() :
			value(0) {
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
		return value < outro.value;
	}
};

#endif /* ITEM_H_ */
