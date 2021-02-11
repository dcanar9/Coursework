#pragma once
#include <iostream>
#include "Card.h"
#include "Deck.h"
#include <vector>

using namespace std;

class Hand {

private:
	vector<card>myHand;


public:
	Hand();
	void pickUpCard(card c);
	card playCard();
	void burnMyCard();
	bool emptyHand();




};