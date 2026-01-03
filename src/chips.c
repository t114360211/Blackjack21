#include "chips.h"

void chips_init(Chips* c, int starting_balance)
{
    if (starting_balance < 0) {  // 防止負數起始餘額
        starting_balance = 0;
    }
    c->balance = starting_balance;
    c->bet = 0;
}

bool chips_can_bet(const Chips* c, int amount)
{
    if (amount <= 0) return false;  // 金額必須為正
    return c->balance >= amount;  // 餘額必須足夠
}

bool chips_place_bet(Chips* c, int amount)
{
    if (!chips_can_bet(c, amount)) return false;  // 檢查是否可下注
    c->balance -= amount;  // 從餘額扣除
    c->bet += amount;  // 加到下注
    return true;
}

void chips_clear_bet(Chips* c)
{
    c->bet = 0;  // 清空下注(不退還)
}

void chips_return_bet(Chips* c)
{
    c->balance += c->bet;  // 退還下注到餘額
    c->bet = 0;  // 清空下注
}

void chips_payout_win(Chips* c)
{
    c->balance += c->bet * 2;  // 支付2倍(本金+獲利)
    chips_clear_bet(c);
}

void chips_payout_push(Chips* c)
{
    c->balance += c->bet;  // 返還本金
    chips_clear_bet(c);
}

void chips_payout_blackjack(Chips* c)
{
    int payout = (c->bet * 5) / 2;  // 計算2.5倍支付
    c->balance += payout;
    chips_clear_bet(c);
}

void chips_payout_lose(Chips* c)
{
    chips_clear_bet(c);  // 只清空下注(已扣除)
}

int chips_get_balance(const Chips* c)
{
    return c->balance;  // 返回當前餘額
}

int chips_get_bet(const Chips* c)
{
    return c->bet;  // 返回當前下注
}
