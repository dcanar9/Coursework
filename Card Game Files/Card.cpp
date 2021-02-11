#include "Card.h"

card::card()
{
	value = 0;
	suit = 'X';
}

card::card(int v, char s)
{
	value = v;
	suit = s;
}

int card::getValue() const
{
	return value;
}

char card::getSuit() const
{
	return suit;
}

void card::setValue(int v)
{
	value = v;
}

void card::setSuit(char s)
{
	suit = s;
}

bool card::operator>(const card & c) const
{
	if (value > c.getValue())
	return true;
	if (value < c.getValue())
	return false;
	if (suit == 's')
		return true;
	if (suit == 'h'&& c.getSuit() != 's')
		return true;
	if (suit == 'd' && c.getSuit() != 'c')
		return false;
	if (suit == 'c')
		return false;
	if (suit == 'd' && c.getSuit() =='c')
		return true;
	if (suit == 'h' && c.getSuit() =='s')
		return false;

}
