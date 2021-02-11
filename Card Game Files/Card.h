#pragma once
#include <iostream>

using namespace std;

class card {

private:
	int value;
	char suit;

public:
	card();
	card(int v, char s);
	int getValue()const;
	char getSuit()const;
	void setValue(int v);
	void setSuit(char s);
	bool operator>(const card & t)const;

	friend ostream& operator<<(ostream& out, const card& c) {
//		if (c.getValue() > 11)
			out << "Value: " << c.getValue() << " " << "Suit: " << c.getSuit();
//		else if (c.getValue() == 11)
//			out << "Value: " << "Jack" << " " << "Suit: " << c.getSuit();
//		else if (c.getValue() == 12)
//			out << "Value: " << "Queen" << " " << "Suit: " << c.getSuit();
//		else if (c.getValue() == 13)
//			out << "Value: " << "King" << " " << "Suit: " << c.getSuit();
//		else if (c.getValue()) == 14)
//			out << "Value: " << "Ace" << " " << "Suit: " << c.getSuit();
		return out;
	}



};