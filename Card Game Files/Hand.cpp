#include "Hand.h"
#include "Deck.h"
#include "Card.h"

Hand::Hand()
{
	myHand;
}

void Hand::pickUpCard(card c)
{
	myHand.push_back(c);
}

card Hand::playCard()
{
	return myHand[0];
}

void Hand::burnMyCard()
{
	myHand.erase(myHand.begin());
}

bool Hand::emptyHand()
{
	if (myHand.size() == 0)
		return true;
	else
		return false;
}
