#pragma once
#ifndef CHIPS_H
#define CHIPS_H
#include <stdbool.h>

typedef struct Chips
{
    int balance;  // 可用餘額
    int bet;      // 當前下注
} Chips;

void chips_init(Chips* c, int starting_balance);  // 初始化籌碼系統
bool chips_can_bet(const Chips* c, int amount);  // 檢查是否可下注
bool chips_place_bet(Chips* c, int amount);  // 進行下注
void chips_clear_bet(Chips* c);  // 清空下注
void chips_return_bet(Chips* c);  // 收回下注到餘額
void chips_payout_win(Chips* c);  // 贏的支付 (1:1)
void chips_payout_push(Chips* c);  // 平局返還
void chips_payout_blackjack(Chips* c);  // 二十一點支付 (3:2)
void chips_payout_lose(Chips* c);  // 輸的處理
int chips_get_balance(const Chips* c);  // 獲取餘額
int chips_get_bet(const Chips* c);  // 獲取下注金額

#endif
