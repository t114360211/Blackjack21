#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include "game.h"
#include "chips.h"
#include "raylib.h"

typedef enum GuiAction
{
    GUI_ACTION_NONE = 0,
    GUI_ACTION_BET_10 = 1,
    GUI_ACTION_BET_50 = 2,
    GUI_ACTION_BET_100 = 3,
    GUI_ACTION_DEAL = 4,
    GUI_ACTION_HIT = 5,
    GUI_ACTION_STAND = 6,
    GUI_ACTION_NEW_ROUND = 7
} GuiAction;

typedef struct Gui
{
    int screen_w;
    int screen_h;
    Font font;
} Gui;

void gui_init(Gui* ui, int w, int h);
void gui_shutdown(Gui* ui);
GuiAction gui_update_and_draw(Gui* ui, const Game* g, const Chips* c);

#endif
