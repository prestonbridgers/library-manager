#ifndef CLM_H_INCLUDED
#define CLM_H_INCLUDED

#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>

#define MAIN_MENU_NUM_ENTRIES 5
#define INSERT_FORM_NUM_FIELDS 10
#define WIN_MAIN_NUM_COLUMNS 5

typedef struct
{
    uint8_t isRunning;

    WINDOW *win_main;
    WINDOW *win_insert;
    WINDOW *win_remove;
    WINDOW *win_about;

    PANEL *pnl_main;
    PANEL *pnl_insert;
    PANEL *pnl_remove;
    PANEL *pnl_about;

    MENU *menu_main;
    char *menu_main_itemNames[MAIN_MENU_NUM_ENTRIES];
    ITEM **menu_main_items;
    char *win_main_columns[WIN_MAIN_NUM_COLUMNS];

    FORM *form_insert;
    FIELD **form_insert_fields;
    char *form_insert_fieldNames[INSERT_FORM_NUM_FIELDS];

    char *title_main;
    char *title_insert;
    char *title_remove;
    char *title_about;

    uint8_t x_main;
    uint8_t x_insert;
    uint8_t x_remove;
    uint8_t x_about;

    uint8_t y_main;
    uint8_t y_insert;
    uint8_t y_remove;
    uint8_t y_about;

    uint8_t w_main;
    uint8_t w_insert;
    uint8_t w_remove;
    uint8_t w_about;

    uint8_t h_main;
    uint8_t h_insert;
    uint8_t h_remove;
    uint8_t h_about;
} LM_STATE;

LM_STATE*   lm_initState();
void        lm_drawMainWin(LM_STATE *s);
void        lm_drawInsertWin(LM_STATE *s);

#endif
