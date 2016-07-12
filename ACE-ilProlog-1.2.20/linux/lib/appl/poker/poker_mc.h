#pragma once

#define MY_DEBUG 0

#define PREFLOP 0
#define FLOP 1
#define TURN 2
#define RIVER 3

#define STRAIGHT_FLUSH 9
#define FOUR_OF_A_KIND  8
#define FULL_HOUSE 7
#define FLUSH 6
#define STRAIGHT 5
#define THREE_OF_A_KIND 4
#define TWO_PAIR 3
#define PAIR 2
#define HIGH_CARD 1

#define THIS_PLAYER 0
#define MAX_PLAYERS 10

#define NUM_CARDS 52
#define MAX_CARDS 7
#define NUM_COMMUNITYCARDS 5
#define NUM_POCKETCARDS 2

#define NUM_SUITS 4
#define NUM_RANKS 13

#define UNKNOWN 0
#define AHEAD 1
#define BEHIND 2
#define TIE 3
#define WINNER 4

#define WIN_ODDS 0
#define HAND_STRENGTH 1
#define HAND_POTENTIAL 2
#define HAND_EFFECTIVE_STRENGTH 3
#define HAND_OUTS 4

#define CARD int

void printRank(int);
void printSuit(int);
void initRand();

class Card {
public:
	int rank;
	int suit;
	void print();
	int operator<(const Card &b) const;
};

class Result {
public:
	Result();
	float forPlayer[MAX_PLAYERS][4];
	float win_odds;
	float hand_strength;
	float hand_potential;
	float hand_effective_strength;
	float hand_outs;
};

class PokerTable {

	public: 
			bool availableCards[NUM_RANKS][NUM_SUITS];    
			Card playerHands[MAX_PLAYERS][NUM_POCKETCARDS];
			bool knownCardsPlayer[MAX_PLAYERS];
			int numKnownCardsPlayer;
			int numPlayers;
			Card communityCards[NUM_COMMUNITYCARDS];
			bool knownCommunityCards[NUM_COMMUNITYCARDS];
			int numKnownCommunityCards;
			Card dealCard();
			void dealPockets(int);
			void assignPockets(int, int, int, int, int);
			void assignCommunityCard(int, int, int);
			PokerTable(int);
			PokerTable(PokerTable *);
			Result eval(int);
			Result simulate(int);
};

class  Combination
{
	public:
		int rank;
		int nbCardsOfRank;
		int operator<(const Combination &b) const;
};

class PokerSet {
	
	public: 
			Card set[MAX_CARDS];
			int nbCards;
			void dealRandomCards(int);
			float eval();
			int findHighestRank();
			void findNumCardsOfRanks(int *);
			int findNumUniqueRanks(int *);
			int findMaxOfRanks(int *);
			PokerSet findFlush();
			int findFlushColor();
			int findStraight();
			PokerSet(int);
			PokerSet(PokerSet *);
			void printPockets();
			void printBoard();
};
