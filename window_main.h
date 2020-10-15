#ifndef WINDOW_MAIN_INCLUDED
#define WINDOW_MAIN_INCLUDED

#include <ncurses.h>
#include <menu.h>

typedef struct
{
    WINDOW *win;
    MENU *menu;
    ITEM **menu_items;

    float scale;
    char *title;

    int n_menu_items;

    int n_cols;

    int n_records;
    char *menu_item_list[5];
    char *cols[5];
} lm_MainWindow;

lm_MainWindow *lm_createMainWindow();
void lm_drawMainWindowColumns(lm_MainWindow *lm);
void lm_addRecord(lm_MainWindow *mw, StringList *record);
void lm_menu_browse();
void lm_menu_insert();
void lm_menu_remove();
void lm_menu_quit();

#endif
