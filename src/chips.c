
#include "chips.h"

void chips_init(Chips* c, int starting_balance)
{
    if (starting_balance < 0) {  // ����t�ư_�l�l�B
        starting_balance = 0;
    }
    c->balance = starting_balance;
    c->bet = 0;
}

bool chips_can_bet(const Chips* c, int amount)
{
    if (amount <= 0) return false;  // ���B��������
    return c->balance >= amount;  // �l�B��������
}

bool chips_place_bet(Chips* c, int amount)
{
    if (!chips_can_bet(c, amount)) return false;  // �ˬd�O�_�i�U�`
    c->balance -= amount;  // �q�l�B����
    c->bet += amount;  // �[��U�`
    return true;
}

void chips_clear_bet(Chips* c)
{
    c->bet = 0;  // �M�ŤU�`(���h��)
}

void chips_return_bet(Chips* c)
{
    c->balance += c->bet;  // �h�٤U�`��l�B
    c->bet = 0;  // �M�ŤU�`
}

void chips_payout_win(Chips* c)
{
    c->balance += c->bet * 2;  // ��I2��(����+��Q)
    chips_clear_bet(c);
}

void chips_payout_push(Chips* c)
{
    c->balance += c->bet;  // ���٥���
    chips_clear_bet(c);
}

void chips_payout_blackjack(Chips* c)
{
    int payout = (c->bet * 5) / 2;  // �p��2.5����I
    c->balance += payout;
    chips_clear_bet(c);
}

void chips_payout_lose(Chips* c)
{
    chips_clear_bet(c);  // �u�M�ŤU�`(�w����)
}

int chips_get_balance(const Chips* c)
{
    return c->balance;  // ��^���e�l�B
}

int chips_get_bet(const Chips* c)
{
    return c->bet;  // ��^���e�U�`
}
