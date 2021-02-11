#include "Deck.h"
#include <ctime>

void Deck::buildDeck()
{
	for (int i = 2; i <= 14; i++) {
		theDeck.push_back(card(i, 'c'));
		theDeck.push_back(card(i, 'h'));
		theDeck.push_back(card(i, 'd'));
		theDeck.push_back(card(i, 's'));
	}
}

Deck::Deck()
{
	buildDeck();
}

void Deck::shuffle()
{
	srand(time(NULL));
	for (int i = theDeck.size() - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		//This is where we will swap the positions of the card
		card temp = theDeck[i];
		theDeck[i] = theDeck[j];
		theDeck[j] = temp;
	}
}

card Deck::dealCard()
{
	return theDeck[0];
}

void Deck::burnCard()
{
	theDeck.erase(theDeck.begin());
}


bool Deck::emptyDeck()
{
	if (theDeck.size() == 0)
		return true;
	else
		return false;
}
