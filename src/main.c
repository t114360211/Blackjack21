#include "raylib.h"
#include "game.h"
#include "chips.h"
#include "gui.h"

static void apply_payout_if_needed(const Game* g, Chips* c)
{
    if (g->state != ROUND_RESULT) return;

    switch (g->outcome)
    {
    case OUTCOME_PLAYER_WIN:
    case OUTCOME_DEALER_BUST:
        chips_payout_win(c);
        break;
    case OUTCOME_PUSH:
        chips_payout_push(c);
        break;
    case OUTCOME_BLACKJACK:
        chips_payout_blackjack(c);
        break;
    case OUTCOME_DEALER_WIN:
    case OUTCOME_PLAYER_BUST:
        chips_payout_lose(c);
        break;
    default:
        break;
    }
}

int main(void)
{
    const int W = 740;
    const int H = 460;

    InitWindow(W, H, "Blackjack 21");
    SetTargetFPS(60);

    Game g;
    Chips c;
    Gui ui;

    game_init(&g);
    chips_init(&c, 1000);
    gui_init(&ui, W, H);

    while (!WindowShouldClose())
    {
        GuiAction act = gui_update_and_draw(&ui, &g, &c);

        if (g.state == ROUND_BETTING)
        {
            if (act == GUI_ACTION_BET_10) chips_place_bet(&c, 10);
            if (act == GUI_ACTION_BET_50) chips_place_bet(&c, 50);
            if (act == GUI_ACTION_BET_100) chips_place_bet(&c, 100);

            if (act == GUI_ACTION_DEAL)
            {
                if (c.bet > 0)
                {
                    game_place_initial_cards(&g);
                }
            }
        }
        else if (g.state == ROUND_PLAYER_TURN)
        {
            if (act == GUI_ACTION_HIT) game_player_hit(&g);
            if (act == GUI_ACTION_STAND) game_player_stand(&g);
        }
        else if (g.state == ROUND_DEALER_TURN)
        {
            if (act == GUI_ACTION_STAND)
            {
                game_dealer_play(&g);
            }
        }
        else if (g.state == ROUND_RESULT)
        {
            apply_payout_if_needed(&g, &c);

            if (act == GUI_ACTION_NEW_ROUND)
            {
                game_new_round(&g);
            }
        }

        if (c.balance <= 0 && g.state == ROUND_BETTING)
        {
            c.balance = 1000;
            c.bet = 0;
        }
    }

    gui_shutdown(&ui);
    CloseWindow();
    return 0;
}
