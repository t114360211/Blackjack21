#include "chips.h"

void chips_init(Chips* c, int starting_balance)
{
    c->balance = starting_balance;
    c->bet = 0;
}

bool chips_can_bet(const Chips* c, int amount)
{
    if (amount <= 0) return false;
    return c->balance >= amount;
}

bool chips_place_bet(Chips* c, int amount)
{
    if (!chips_can_bet(c, amount)) return false;
    c->balance -= amount;
    c->bet += amount;
    return true;
}

void chips_clear_bet(Chips* c)
{
    c->bet = 0;
}

void chips_payout_win(Chips* c)
{
    c->balance += c->bet * 2;
    chips_clear_bet(c);
}

void chips_payout_push(Chips* c)
{
    c->balance += c->bet;
    chips_clear_bet(c);
}

void chips_payout_blackjack(Chips* c)
{
    int payout = (c->bet * 5) / 2;
    c->balance += payout;
    chips_clear_bet(c);
}

void chips_payout_lose(Chips* c)
{
    chips_clear_bet(c);
}
