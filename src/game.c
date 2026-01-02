#include "game.h"
#include <stdlib.h>
#include <time.h>

static void hand_clear(Hand* h)
{
    h->count = 0;
}

static void hand_add(Hand* h, Card c)
{
    if (h->count < MAX_HAND_CARDS)
    {
        h->cards[h->count] = c;
        h->count += 1;
    }
}

static void deck_build(Card deck[52])
{
    int idx = 0;
    for (int s = 0; s < 4; s++)
    {
        for (int r = 1; r <= 13; r++)
        {
            deck[idx].rank = r;
            deck[idx].suit = (Suit)s;
            idx += 1;
        }
    }
}

static void deck_shuffle(Card deck[52])
{
    for (int i = 51; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

static Card deck_draw(Game* g)
{
    if (g->deck_index >= 52)
    {
        deck_build(g->deck);
        deck_shuffle(g->deck);
        g->deck_index = 0;
    }
    Card c = g->deck[g->deck_index];
    g->deck_index += 1;
    return c;
}

int game_hand_value(const Hand* h)
{
    int total = 0;
    int aces = 0;

    for (int i = 0; i < h->count; i++)
    {
        int r = h->cards[i].rank;
        if (r == 1)
        {
            total += 11;
            aces += 1;
        }
        else if (r >= 10)
        {
            total += 10;
        }
        else
        {
            total += r;
        }
    }

    while (total > 21 && aces > 0)
    {
        total -= 10;
        aces -= 1;
    }

    return total;
}

bool game_is_blackjack(const Hand* h)
{
    if (h->count != 2) return false;
    return game_hand_value(h) == 21;
}

bool game_is_bust(const Hand* h)
{
    return game_hand_value(h) > 21;
}

void game_init(Game* g)
{
    srand((unsigned)time(NULL));
    deck_build(g->deck);
    deck_shuffle(g->deck);
    g->deck_index = 0;

    hand_clear(&g->player);
    hand_clear(&g->dealer);

    g->state = ROUND_BETTING;
    g->outcome = OUTCOME_NONE;
    g->player_stand = false;
}

void game_new_round(Game* g)
{
    hand_clear(&g->player);
    hand_clear(&g->dealer);
    g->state = ROUND_BETTING;
    g->outcome = OUTCOME_NONE;
    g->player_stand = false;
}

void game_place_initial_cards(Game* g)
{
    hand_clear(&g->player);
    hand_clear(&g->dealer);

    hand_add(&g->player, deck_draw(g));
    hand_add(&g->dealer, deck_draw(g));
    hand_add(&g->player, deck_draw(g));
    hand_add(&g->dealer, deck_draw(g));

    g->state = ROUND_PLAYER_TURN;
    g->outcome = OUTCOME_NONE;
    g->player_stand = false;

    if (game_is_blackjack(&g->player) && game_is_blackjack(&g->dealer))
    {
        g->outcome = OUTCOME_PUSH;
        g->state = ROUND_RESULT;
    }
    else if (game_is_blackjack(&g->player))
    {
        g->outcome = OUTCOME_BLACKJACK;
        g->state = ROUND_RESULT;
    }
    else if (game_is_blackjack(&g->dealer))
    {
        g->outcome = OUTCOME_DEALER_WIN;
        g->state = ROUND_RESULT;
    }
}

void game_player_hit(Game* g)
{
    if (g->state != ROUND_PLAYER_TURN) return;
    hand_add(&g->player, deck_draw(g));

    if (game_is_bust(&g->player))
    {
        g->outcome = OUTCOME_PLAYER_BUST;
        g->state = ROUND_RESULT;
    }
}

void game_player_stand(Game* g)
{
    if (g->state != ROUND_PLAYER_TURN) return;
    g->player_stand = true;
    g->state = ROUND_DEALER_TURN;
}

void game_dealer_play(Game* g)
{
    if (g->state != ROUND_DEALER_TURN) return;

    while (game_hand_value(&g->dealer) < 17)
    {
        hand_add(&g->dealer, deck_draw(g));
    }

    if (game_is_bust(&g->dealer))
    {
        g->outcome = OUTCOME_DEALER_BUST;
        g->state = ROUND_RESULT;
        return;
    }

    int pv = game_hand_value(&g->player);
    int dv = game_hand_value(&g->dealer);

    if (pv > dv) g->outcome = OUTCOME_PLAYER_WIN;
    else if (pv < dv) g->outcome = OUTCOME_DEALER_WIN;
    else g->outcome = OUTCOME_PUSH;

    g->state = ROUND_RESULT;
}
