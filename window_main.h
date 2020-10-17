#ifndef WINDOW_MAIN_INCLUDED
#define WINDOW_MAIN_INCLUDED

#include <ncurses.h>
#include <menu.h>
#include "string_list.h"

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

/* Functions I need:
 *
 * lm_createMainWindow();   // Creates and returns a pointer to a lm_MainWindow struct
 * lm_addRecord();          // Adds a record to the main window display
 * 
*/

lm_MainWindow *lm_createMainWindow();
void lm_redrawMainWindow(lm_MainWindow *lm);
void lm_addRecord(lm_MainWindow *mw, StringList *record);
void lm_menu_browse();
StringList *lm_menu_insert();
void lm_menu_remove();
void lm_menu_quit();

#endif
