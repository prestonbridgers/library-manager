#ifndef WINDOW_MAIN_INCLUDED
#define WINDOW_MAIN_INCLUDED

#include <ncurses.h>
#include <menu.h>

typedef struct
{
    WINDOW *win;
    MENU *menu;
    ITEM **menu_items;
} lm_MainWindow;

lm_MainWindow *lm_createMainWindow();
void lm_fillMainWindowColumns(lm_MainWindow *lm, uint8_t starty, uint8_t startx, char *data[], int columns);
void lm_menu_browse();
void lm_menu_insert();
void lm_menu_remove();
void lm_menu_quit();

#endif
