
#pragma once
#ifndef CHIPS_H
#define CHIPS_H
#include <stdbool.h>

typedef struct Chips
{
    int balance;  // �i�ξl�B
    int bet;      // ���e�U�`
} Chips;

void chips_init(Chips* c, int starting_balance);  // ��l���w�X�t��
bool chips_can_bet(const Chips* c, int amount);  // �ˬd�O�_�i�U�`
bool chips_place_bet(Chips* c, int amount);  // �i��U�`
void chips_clear_bet(Chips* c);  // �M�ŤU�`
void chips_return_bet(Chips* c);  // ���^�U�`��l�B
void chips_payout_win(Chips* c);  // Ĺ����I (1:1)
void chips_payout_push(Chips* c);  // ��������
void chips_payout_blackjack(Chips* c);  // �G�Q�@�I��I (3:2)
void chips_payout_lose(Chips* c);  // �骺�B�z
int chips_get_balance(const Chips* c);  // ����l�B
int chips_get_bet(const Chips* c);  // ����U�`���B

#endif
