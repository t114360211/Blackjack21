#ifndef GAME_H
#define GAME_H

#include "common.h"

typedef enum RoundState
{
    ROUND_BETTING = 0,
    ROUND_PLAYER_TURN = 1,
    ROUND_DEALER_TURN = 2,
    ROUND_RESULT = 3
} RoundState;

typedef enum RoundOutcome
{
    OUTCOME_NONE = 0,
    OUTCOME_PLAYER_WIN = 1,
    OUTCOME_DEALER_WIN = 2,
    OUTCOME_PUSH = 3,
    OUTCOME_PLAYER_BUST = 4,
    OUTCOME_DEALER_BUST = 5,
    OUTCOME_BLACKJACK = 6
} RoundOutcome;

typedef struct Game
{
    Card deck[52];
    int deck_index;

    Hand player;
    Hand dealer;

    RoundState state;
    RoundOutcome outcome;

    bool player_stand;
} Game;

void game_init(Game* g);
void game_new_round(Game* g);
void game_place_initial_cards(Game* g);

void game_player_hit(Game* g);
void game_player_stand(Game* g);
void game_dealer_play(Game* g);

int game_hand_value(const Hand* h);
bool game_is_blackjack(const Hand* h);
bool game_is_bust(const Hand* h);

#endif
