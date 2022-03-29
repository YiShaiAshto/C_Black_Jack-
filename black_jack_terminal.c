# include <stdio.h>  
# include <stdint.h>  
# include <stdlib.h>  
# include <time.h>  

 # define DECK_MAX_LEN  52

//-----------------------------------------------
//              *  Structs  *
// ----------------------------------------------
typedef struct Card Card;
struct Card { 
    uint8_t face : 4,    //   4 bits; 0-15 LSB
            suit : 2,    //   2 bits; 0-3 
            color : 1;   //   1 bit; 0-1  MSB
    Card *nexp;
};

typedef struct  List{
    Card *head;
    size_t len; 
} CardList;

//-----------------------------------------------
//              *  PROTOTYPES  *
//-----------------------------------------------
void shuffle(CardList * deck);
void fill_deck(CardList * deck); 
void list_init(CardList *list);  
Card * remove_card(CardList * list);
void print_cards(CardList * card_list ); 
int get_cards_value(CardList *card_list);  
void add_card(CardList * list, Card * new_card); 
void insert(CardList *list, Card *n, size_t pos);
void lose_phase(size_t * bet ,size_t * cash);
void betting_phase(size_t * cash, size_t * bet);
void deal_card(CardList * deck, CardList * hand);
void reset_cards(CardList * player, CardList * dealer, CardList * deck);
void print_dealer_cards(CardList * card_list, size_t num_of_dealer_cards);
void hit_or_stand_phase(CardList * player, CardList * dealer, CardList * deck, size_t * cash, size_t * bet);
void dealer_draw_phase(CardList * player, CardList * dealer, CardList * deck,  size_t * cash, size_t * bet);

//-----------------------------------------------
//             *  MAIN  *
//-----------------------------------------------
int main(int argc, char const *argv[])
{
    // initializations
    size_t cash = 1000, bet = 0;      // Cash & Bet;
    srand(time(NULL));                // Seeding the random number genrator.  
    int keep_playing = 1;             // Keep play [1], Stop [0].
    int res = 0;                      // help to check if scanf input is int.

    CardList deck;
    CardList dealer_hand;
    CardList player_hand;

    list_init(&deck);
    list_init(&dealer_hand);
    list_init(&player_hand);

    fill_deck(&deck);

    printf("\nCards in deck before shuffle:\n");
    print_cards(&deck);
    shuffle(&deck);
    printf("\nCards in deck after shuffle:\n");
    print_cards(&deck);

    // Black Jack
    while (keep_playing)
    {
        // Initial Deal phase
        deal_card(&deck, &player_hand);
        deal_card(&deck, &player_hand);
        deal_card(&deck, &dealer_hand);
        deal_card(&deck, &dealer_hand);

        puts("");

        if (cash < 10)
        {
            puts("out of cash for bet. Game Over.");
            break;
        }    

        betting_phase(&cash, &bet);
        printf("\nDelaer : ");
        print_dealer_cards(&dealer_hand, dealer_hand.len);
        printf("\nPlayer : ");
        print_cards(&player_hand);  

        if (get_cards_value(&player_hand) == 21)
        {
            puts("AMAZING BLACK JACK!");
            cash = cash + bet * 2.5;
            bet = 0;
            reset_cards(&player_hand, &dealer_hand, &deck);
            shuffle(&deck);
        }
        else
            hit_or_stand_phase(&player_hand, &dealer_hand, &deck, &cash, &bet);

        printf("Status: cash = %zu bet = %zu", cash, bet);        
        printf("\nWould you like to play again ? [1/0] --> ");
        res = scanf("%d", &keep_playing);
        while (res != 1) 
        { 
            if (res != 1){
                while ((getchar()) != '\n'); // clears the buffer.
                puts("Invalid input! try [1/0]");
                }
            res = scanf("%d", &keep_playing);    
        }
        list_init(&player_hand);
        list_init(&dealer_hand);
    }
    puts("Thanks for playing!");

    while (deck.head != NULL) // free the deck of card from memo.
    {          
        Card * temp = deck.head->nexp;
        free(deck.head);
        deck.head = temp;
    }
    return 0;
}

//-----------------------------------------------
//              *  FUNCTIONS  *
//-----------------------------------------------
// Initialize list.
void list_init(CardList *list) 
{
    list->head = NULL;
    list->len = 0;
}

// Creating  deck of cards.
void fill_deck(CardList * deck)
{
    for (size_t i = 0; i < DECK_MAX_LEN; ++i) 
    {
        // initialize the cards togther
        Card * new_card = malloc(sizeof(Card));
        new_card->face = i % (DECK_MAX_LEN / 4);;
        new_card->suit = i / (DECK_MAX_LEN / 4);;
        new_card->color = i / (DECK_MAX_LEN / 2);;
    
        // linking the cards togther
        new_card->nexp = deck->head;
        deck->head = new_card;
        deck->len++;
    }
}

// Prints the cards in a list.
void print_cards(CardList * card_list)
{
    Card * n = card_list->head;
    size_t i = 1, z = 0;
    const char * face[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9",
                    "10", "Joker",  "Queen", "King"}; 
    const char * suit[] = {"Hearts", "Dimonds", "Clubs", "Spades"}; 
    const char * color[] = {"Red", "Black"};

    printf("have %zu cards.  \n", card_list->len);

    while (n != NULL)
    {
        printf("Card %zu: ", i++);
        z = n->face;       
        printf("%s ", face[z]);
        z = n->suit;       
        printf("%s ", suit[z]);
        z = n->color;
        printf("%s ", color[z]);
        puts("");         
        n = n->nexp;
    }  
}

// Get list of cards, and calculate it’s value and returns it.
int get_cards_value(CardList *card_list)
{
    Card *n = card_list->head; 
    size_t sum = 0;
    while (n != NULL)
    {
        switch(n->face)
        {
            case 0:
                if ((sum += n->face) <= 10)
                    sum += 11;
                else
                    sum += 1;     
                break;
            case 1:
                sum += 2;
                break;
            case 2:
                sum += 3;
                break;
            case 3:
                sum += 4;
                break;
            case 4:
                sum += 5;
                break;
            case 5:
                sum += 6;
                break;
            case 6:
                sum += 7;
                break;
            case 7:
                sum += 8;
                break;
            case 8:
                sum += 9;
                break;
            case 9:
                sum += 10;
                break;
            case 10:
                sum += 10;
                break;
            case 11:
                sum += 10;
                break;
            case 12:
                sum += 10;
                break;   
                default:
                    break;
        }
        n = n->nexp;    
    }
    return sum;
}

// Add(append) card to player/dealer from right side.
// This function helps to "deal_card" function.       
void add_card(CardList * list, Card * new_card)
{
    Card * prev = NULL;
    Card * curr = list->head;

    while(curr != NULL) {
        prev = curr;
        curr = curr->nexp;
    }

    new_card->nexp = NULL;

    //if curr is pointing to head.
    if (prev == NULL)
        list->head = new_card;
    else 
        prev->nexp = new_card;
    list->len++;    
}

// Deal 1 card from deck at random postion. 
// also remove from the list of deck.
void deal_card(CardList * deck, CardList * hand)
{    
    Card * prev = NULL;
    Card * curr = deck->head;
    size_t steps = 0, position = rand()%deck->len;

    while (steps < position) 
    { 
        prev = curr;
        curr = curr->nexp;
        steps++;
    }

    if (prev) prev->nexp = curr->nexp;
    else deck->head = curr->nexp;

    curr->nexp = NULL;
    deck->len--;

    add_card(hand, curr);
}

// Prints the cards of dealer without showing the last hand.
void print_dealer_cards(CardList * card_list, size_t num_of_dealer_cards)
{
    size_t i = 1, z = 0;
    Card * n = card_list->head;
 
    const char * face[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9",
                    "10", "Joker",  "Queen", "King"}; 
    const char * suit[] = {"Hearts", "Dimonds", "Clubs", "Spades"}; 
    const char * color[] = {"Red", "Black"};

    printf("have %zu cards.\n", card_list->len);

    while (n != NULL && i < num_of_dealer_cards)
    {
        printf("Card %zu: ", i++);
        z = n->face;       
        printf("%s ", face[z]);
        z = n->suit;       
        printf("%s ", suit[z]);
        z = n->color;
        printf("%s ", color[z]);
        puts("");         
        n = n->nexp;
    }
    printf("Card %zu: ????", i);
    puts("");  
}

// Betting phase.
void betting_phase(size_t * cash, size_t * bet)
{
    // Betting phase
    while (1)
    {
        printf("\nplayer’s cash =%zu, bet =%zu\n", *cash, *bet);
        puts("How much would you like to bet ?");
        int res = scanf("%zu", bet);
        if (res != 1)                        // checks if bet is int type
            {
                puts("Must be an int.");
                while (getchar() != '\n'); // clears the buffer.
                continue;
            }    

        if (*bet > *cash)
        {
            puts("Bet is bigger than cash, can't place the bet.");
            *bet = 0;
            continue;
        }
        if (*bet % 10 != 0)
        {
            puts("Bet Most be in multiples of 10");
            *bet = 0;
            continue;
        }
        *cash -= *bet;
        printf("cash = %zu, bet = %zu\n", *cash, *bet);
        puts("Good luck!");
        break;
    }
}

// Loosing phase. reset the cards and shuffle.
void loosing_phase(CardList * player, CardList * dealer, CardList * deck, size_t * bet)
{
    *bet = 0;
    reset_cards(player, dealer, deck);
    shuffle(deck);
}

// Remove cards from the top of the of hand list.
// this function helps to "reset_cards" function. 
Card * remove_card(CardList * list)
{
    Card * temp = list->head;

    if (list->len ==0)
        return temp;
    else if (list->head !=  NULL)
        list->head = temp->nexp;
           
    list->len--;     
    return temp;
}

// Take the cards from the hand to the deck. 
void reset_cards(CardList * player, CardList * dealer, CardList * deck)
{
    Card * card = NULL;
    
    while (player->len != 0)
    {
        card = remove_card(player);
        add_card(deck, card);
    }
    while (dealer->len != 0)
    {
        card = remove_card(dealer);
        add_card(deck, card);
    }
}

// Dealer draw phase.
void dealer_draw_phase(CardList * player, CardList * dealer, CardList * deck, size_t * cash, size_t * bet)
{
    if (get_cards_value(dealer) > get_cards_value(player))
    {
        printf("\nDelaer : ");
        print_cards(dealer);
        printf("\nPlayer : ");
        print_cards(player); 
        puts("");
        puts("You lose!");
        loosing_phase(player, dealer, deck, bet);
    }
    else
    {
        while (get_cards_value(dealer) < 17 || 
        get_cards_value(dealer) <= get_cards_value(player))
        {   
            deal_card(deck, dealer);
            if (get_cards_value(dealer) == get_cards_value(player))
            {   
                printf("\nDelaer : ");
                print_cards(dealer);
                printf("\nPlayer : ");
                print_cards(player); 
                puts("");
                puts("It's a TIE!");
                *cash = *cash + *bet;           // brings back the money to player.
                *bet = 0;                       // reset the bet.
                reset_cards(player, dealer, deck);
                shuffle(deck);
                return;
            }    
            else if (get_cards_value(dealer) > 21)
            {
                printf("\nDelaer : ");
                print_cards(dealer);
                printf("\nPlayer : ");
                print_cards(player);  
                puts("");
                puts("Delaer BUST!");
                *cash = *cash + *bet * 2;
                *bet = 0;
                reset_cards(player, dealer, deck);
                shuffle(deck);
                return;
            }
            else if (get_cards_value(dealer) == 21 || 
            get_cards_value(dealer) > get_cards_value(player))
            {
                printf("\nDelaer : ");
                print_cards(dealer);
                printf("\nPlayer : ");
                print_cards(player);   
                puts("");      
                puts("Dealer WINS!");
                loosing_phase(player, dealer, deck, bet);
                return;
            }
        }
    }
}

//  Hit or stand phase.
void hit_or_stand_phase(CardList * player, CardList * dealer, CardList * deck, size_t * cash, size_t * bet)
{
    char h_or_s;
    while (1)
    {
        while ((getchar()) != '\n');
        printf("Choose: hit or stand ? [h/s]  --> ");
        scanf("%c", &h_or_s);
        puts("");
        if (h_or_s == 's')
            {
                dealer_draw_phase(player, dealer, deck, cash, bet); 
                break;
            }
        else if (h_or_s == 'h')
        {
            deal_card(deck, player);
            printf("Delaer : ");
            print_dealer_cards(dealer, dealer->len);
            printf("\nPlayer : ");
            print_cards(player);
            if (get_cards_value(player) > 21)
                {
                    puts("");
                    puts("BUST!");
                    loosing_phase(player, dealer, deck, bet);
                    break;
                }
            else if (get_cards_value(player) <= 21)
                continue;    
        }        
    }
}

// Insert 1 card at givin postion --> Credit to Shmule ;)
// this function help to "shuffle" function.
void insert(CardList *list, Card *n, size_t pos) {
    //travel pos steps (or stop if reached the end first)
    Card *prev = NULL;
    Card *curr = list->head;
    size_t steps = 0;

    while (steps < pos && curr != NULL) {
        prev = curr;
        curr = curr->nexp;
    }

    if (prev) prev->nexp = n;
    else list->head = n;

    n->nexp = curr;
}

//  Shuffle  card from deck at random postion 
//  and insert it back random postion to deck.
void shuffle(CardList * deck)
{
    // This part of the shfulle will travle to random card postion to remove.
    Card * prev = NULL;
    Card * curr = deck->head;
    size_t i = 1;
    while(i ++ < DECK_MAX_LEN)       
    {
        size_t steps = 0;

        while (steps++ < rand () % deck->len) 
        { 
            prev = curr;
            curr = curr->nexp;
        }

        if (prev) prev->nexp = curr->nexp;
        else deck->head = curr->nexp;
            
        curr->nexp = NULL;

        insert(deck, curr, rand() % deck->len);

        prev = NULL;
        curr = deck->head;
    }
}