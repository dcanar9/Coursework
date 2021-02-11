//Dennis Canar
//10-8-17
//CST-210 Fritz
//This is my work

#include <iostream>
#include <vector>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"

using namespace std;

int main(){

	Deck d;
	Hand p1, p2;
	card c;
	int p1Points = 0;
	int p2Points = 0;
	d.shuffle();
	d.shuffle();

	//This is where we split the Deck to both the players hands
	while (d.emptyDeck() == false) {
		c = d.dealCard();
		p1.pickUpCard(c);
		d.burnCard();
		c = d.dealCard();
		p2.pickUpCard(c);
		d.burnCard();
	}
	//This is the actual game play and printing occurs
	while (p1.emptyHand()==false && p2.emptyHand() == false) {
		cout << "Player One " << p1.playCard() << " V.S. " << "Player Two " << p2.playCard() << endl;
		
		//This is where we will count the points for the number of rounds won
		if (p1.playCard() > p2.playCard() == true) {
			p1Points++;
			cout << "Player 1 Wins Round" << endl;
		}
		
		//Player Two Wins
		if (p1.playCard() > p2.playCard() == false) {
			p2Points++;
			cout << "Player 2 Wins Round" << endl;
		}
		
		//Remove card from both hands
		p1.burnMyCard();
		p2.burnMyCard();
	}
	
	
	//End Game Results, who wins?
	if (p2.emptyHand() == true) {
		if (p1Points > p2Points)
			cout << "Player One Wins With " << p1Points << " Points" << "| Player Two Loses With " << p2Points << " Points" << endl;
		else if (p1Points == p2Points)
			cout << "It's a Tie!!!" << endl;
		else if (p1Points < p2Points)
			cout << "Player Two Wins With " << p2Points << " Points" << "| Player One Loses With "<< p1Points << " Points" <<endl;
	}



}