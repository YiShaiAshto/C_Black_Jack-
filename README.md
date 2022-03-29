# C_Black_Jack-
A terminal-based game of black jack between a human player and a computer
controlled dealer, using linked list (Data Structure for the cards).

The game will use playing cards. Or simply a “Card” struct. Each card has four fields: suit,
rank & next (Using bitfileds).

The game will start from phase 1 listed below, and when each part is done the game will
move to the next phase. 

1. Game Initialization: 
Creates a card linked list called “deck”. It will represent a deck of 52
standard playing cards, containing one of every combination of rank and suit.
Game Start with two empty card linked lists called “dealer_hand” and “player_hand”
Game Start with a “cash” integer equals to 1000, and “bet” equals 0.

2. Betting phase:
prints the player’s cash, and bet.
The player asked how much they’re willing to add to the bet (can’t be more than cash available).
Bets must be made in multiples of 10. 

3. Initial Deal phase:
Draw 2 cards into the dealer hand, and 2 cards into the player hand. 
Drawing random cards: a random position in the deck list, remove the card in that position and insert it as the last position of the hand.

4. Hit or Stand Phase
a) input the user for either “hit” or “stand”. If the user chooses “stand” move to the next phase (dealer draw phase).
b) If the user chooses “hit”, drawing a card into the player’s hand. Then, calculate the value of the player’s hand. If it’s above 21, the player loses the bet, prints “bust” and move to the “lose bet” phase. If the value is below 21, repeat phase 4.

5. Dealer draw phase
Calculate the value of the hand of the dealer. If it’s greater than the player’s hand value,
that player loses. Otherwise, keep drawing cards until one of the following occurs:
- the dealer’s hand value is greater than the player’s hand value. 
- the dealer’s hand value has reached a value of 17 or more.

6. Reset Cards.
Add all the cards in the player’s and dealer’s hand to the top of the deck. (player and
dealer hand should now be empty. The deck should contain all the cards.
If the player's cash is less than 10, the game is over
Otherwise, The player asked if they wish to bet again (yes/no). If they do, back to phase 2.
Otherwise, end of the program.

Before the program ends, we free all dynamically allocated memory.

