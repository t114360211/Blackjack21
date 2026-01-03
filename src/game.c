#include "game.h"
#include <stdlib.h>
#include <time.h>

// 內部輔助：清空手牌
static void hand_clear(Hand* h)
{
    h->count = 0;
}

// 內部輔助：加牌
static void hand_add(Hand* h, Card c)
{
    if (h->count < MAX_HAND_CARDS)
    {
        h->cards[h->count] = c;
        h->count += 1;
    }
}

// 內部輔助：建立牌組
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

// 內部輔助：洗牌
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

// 內部輔助：抽牌
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

// 核心邏輯：計算點數
int game_hand_value(const Hand* h)
{
    int total = 0;
    int aces = 0;

    for (int i = 0; i < h->count; i++)
    {
        int r = h->cards[i].rank;
        if (r == 1) // Ace
        {
            total += 11;
            aces += 1;
        }
        else if (r >= 10) // J, Q, K
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

// 核心邏輯：是否為 21 點
bool game_is_blackjack(const Hand* h)
{
    if (h->count != 2) return false;
    return game_hand_value(h) == 21;
}

// 核心邏輯：是否爆牌
bool game_is_bust(const Hand* h)
{
    return game_hand_value(h) > 21;
}

// 初始化遊戲環境
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

// 新局準備
void game_new_round(Game* g)
{
    hand_clear(&g->player);
    hand_clear(&g->dealer);
    g->state = ROUND_BETTING;
    g->outcome = OUTCOME_NONE;
    g->player_stand = false;
}

// 發初始牌
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

    // 初始判定
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

// 【修正重點】優化要牌邏輯：處理爆牌、過五關、以及自動停止於 21 點
void game_player_hit(Game* g)
{
    if (g->state != ROUND_PLAYER_TURN) return;

    hand_add(&g->player, deck_draw(g));

    int current_val = game_hand_value(&g->player);

    // 1. 檢查是否爆牌
    if (current_val > 21)
    {
        g->outcome = OUTCOME_PLAYER_BUST;
        g->state = ROUND_RESULT;
    }
    // 2. 檢查是否過五關 (五張牌且未爆牌)
    else if (g->player.count
