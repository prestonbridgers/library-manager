#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "window_main.h"

int is_running = 1;

int main()
{
    // nCurses init
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();

    int input;

    lm_MainWindow *mWin = lm_createMainWindow();

    // Input loop
    while (is_running && (input = getch()) != 'q')
    {
        switch (input)
        {
            case 'j':
            case 'h':
                menu_driver(mWin->menu, REQ_LEFT_ITEM);
                break;
            case 'k':
            case 'l':
                menu_driver(mWin->menu, REQ_RIGHT_ITEM);
                break;
            case 10: /* Enter */
            {
                ITEM *cur = current_item(mWin->menu);
                void (*p)(void) = item_userptr(cur);
                p();
                break;
            }
        }

        wrefresh(mWin->win);
    }

    delwin(mWin->win);
    endwin();
    return 0;
}
