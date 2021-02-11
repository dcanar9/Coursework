#pragma once
#include <iostream>
#include <vector>
#include "Card.h"
#include <ctime>

using namespace std;

class Deck {

private:
	vector<card>theDeck;
	void buildDeck();
public:
	Deck();
	void shuffle();
	card dealCard();
	void burnCard();
	bool emptyDeck();

};