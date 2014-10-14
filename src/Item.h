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
	int contagem = 1;
	int valor;

public:
	Item(int valor) :
			valor(valor) {
	}

	bool operator<(const Item& outro) {
		return valor < outro.valor;
	}
};

#endif /* ITEM_H_ */
