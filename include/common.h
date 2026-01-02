#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_HAND_CARDS 16

typedef enum Suit
{
    SUIT_CLUBS = 0,
    SUIT_DIAMONDS = 1,
    SUIT_HEARTS = 2,
    SUIT_SPADES = 3
} Suit;

typedef struct Card
{
    int rank;
    Suit suit;
} Card;

typedef struct Hand
{
    Card cards[MAX_HAND_CARDS];
    int count;
} Hand;

#endif
