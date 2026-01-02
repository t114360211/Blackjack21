#include "gui.h"
#include <stdio.h>

static const char* suit_to_str(Suit s)
{
    switch (s)
    {
    case SUIT_CLUBS: return "C";
    case SUIT_DIAMONDS: return "D";
    case SUIT_HEARTS: return "H";
    case SUIT_SPADES: return "S";
    default: return "?";
    }
}

static void card_to_text(const Card* c, char out[8])
{
    const char* r = "?";
    if (c->rank == 1) r = "A";
    else if (c->rank == 11) r = "J";
    else if (c->rank == 12) r = "Q";
    else if (c->rank == 13) r = "K";
    else
    {
        static char buf[3];
        snprintf(buf, sizeof(buf), "%d", c->rank);
        r = buf;
    }
    snprintf(out, 8, "%s%s", r, suit_to_str(c->suit));
}

static bool button(Rectangle rect, const char* text)
{
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, rect);
    DrawRectangleRec(rect, hover ? (Color) { 200, 200, 200, 255 } : (Color) { 160, 160, 160, 255 });
    DrawRectangleLinesEx(rect, 2, (Color) { 60, 60, 60, 255 });
    DrawText(text, (int)rect.x + 10, (int)rect.y + 10, 20, BLACK);
    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void gui_init(Gui* ui, int w, int h)
{
    ui->screen_w = w;
    ui->screen_h = h;
    ui->font = GetFontDefault();
}

void gui_shutdown(Gui* ui)
{
    (void)ui;
}

static void draw_hand(int x, int y, const Hand* h, bool hide_first)
{
    for (int i = 0; i < h->count; i++)
    {
        Rectangle r = { (float)(x + i * 70) , (float)y , 60 , 90 };
        DrawRectangleRec(r, (Color) { 245, 245, 245, 255 });
        DrawRectangleLinesEx(r, 2, (Color) { 40, 40, 40, 255 });

        if (hide_first && i == 0)
        {
            DrawText("??", (int)r.x + 18, (int)r.y + 35, 20, DARKGRAY);
        }
        else
        {
            char t[8];
            card_to_text(&h->cards[i], t);
            DrawText(t, (int)r.x + 12, (int)r.y + 35, 20, BLACK);
        }
    }
}

GuiAction gui_update_and_draw(Gui* ui, const Game* g, const Chips* c)
{
    BeginDrawing();
    ClearBackground((Color) { 25, 120, 60, 255 });

    DrawText("Blackjack 21", 20, 20, 28, RAYWHITE);

    char info[256];
    snprintf(info, sizeof(info), "Balance : %d   Bet : %d", c->balance, c->bet);
    DrawText(info, 20, 60, 20, RAYWHITE);

    DrawText("Dealer", 20, 110, 22, RAYWHITE);
    bool hide = (g->state == ROUND_PLAYER_TURN || g->state == ROUND_BETTING);
    draw_hand(20, 140, &g->dealer, hide);

    DrawText("Player", 20, 260, 22, RAYWHITE);
    draw_hand(20, 290, &g->player, false);

    if (g->state != ROUND_BETTING)
    {
        int pv = game_hand_value(&g->player);
        char pvtxt[64];
        snprintf(pvtxt, sizeof(pvtxt), "Player value : %d", pv);
        DrawText(pvtxt, 20, 390, 20, RAYWHITE);
    }

    GuiAction act = GUI_ACTION_NONE;

    int bx = 520;
    int by = 120;

    if (g->state == ROUND_BETTING)
    {
        DrawText("Place bet", bx, by - 30, 20, RAYWHITE);

        if (button((Rectangle) { bx, by, 180, 45 }, "Bet 10")) act = GUI_ACTION_BET_10;
        if (button((Rectangle) { bx, by + 60, 180, 45 }, "Bet 50")) act = GUI_ACTION_BET_50;
        if (button((Rectangle) { bx, by + 120, 180, 45 }, "Bet 100")) act = GUI_ACTION_BET_100;

        if (button((Rectangle) { bx, by + 200, 180, 55 }, "Deal")) act = GUI_ACTION_DEAL;
    }
    else if (g->state == ROUND_PLAYER_TURN)
    {
        DrawText("Your turn", bx, by - 30, 20, RAYWHITE);

        if (button((Rectangle) { bx, by, 180, 55 }, "Hit")) act = GUI_ACTION_HIT;
        if (button((Rectangle) { bx, by + 70, 180, 55 }, "Stand")) act = GUI_ACTION_STAND;
    }
    else if (g->state == ROUND_DEALER_TURN)
    {
        DrawText("Dealer turn", bx, by - 30, 20, RAYWHITE);
        DrawText("Press Space to run dealer", bx, by, 18, RAYWHITE);
        if (IsKeyPressed(KEY_SPACE))
        {
            act = GUI_ACTION_STAND;
        }
    }
    else if (g->state == ROUND_RESULT)
    {
        const char* msg = "Result";
        switch (g->outcome)
        {
        case OUTCOME_PLAYER_WIN: msg = "Player wins"; break;
        case OUTCOME_DEALER_WIN: msg = "Dealer wins"; break;
        case OUTCOME_PUSH: msg = "Push"; break;
        case OUTCOME_PLAYER_BUST: msg = "Player bust"; break;
        case OUTCOME_DEALER_BUST: msg = "Dealer bust"; break;
        case OUTCOME_BLACKJACK: msg = "Blackjack"; break;
        default: msg = "Result"; break;
        }
        DrawText(msg, bx, by - 10, 24, RAYWHITE);

        if (button((Rectangle) { bx, by + 60, 180, 55 }, "New round")) act = GUI_ACTION_NEW_ROUND;
    }

    EndDrawing();
    return act;
}
