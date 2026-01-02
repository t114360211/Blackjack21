#ifndef CHIPS_H
#define CHIPS_H

#include <stdbool.h>

typedef struct Chips
{
    int balance;
    int bet;
} Chips;

void chips_init(Chips* c, int starting_balance);
bool chips_can_bet(const Chips* c, int amount);
bool chips_place_bet(Chips* c, int amount);
void chips_clear_bet(Chips* c);
void chips_payout_win(Chips* c);
void chips_payout_push(Chips* c);
void chips_payout_blackjack(Chips* c);
void chips_payout_lose(Chips* c);

#endif
