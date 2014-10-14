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
	Item(int value) :
			value(value) {
	}

	void increment() {
		count++;
	}

	bool operator<(const Item& outro) {
		return value < outro.value;
	}
};

#endif /* ITEM_H_ */
