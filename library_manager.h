#ifndef LIBRARY_MANAGER_H_INCLUDED
#define LIBRARY_MANAGER_H_INCLUDED

#include <ncurses.h>
#include <mysql.h>
#include <menu.h>
#include <form.h>
#include "string_list.h"

typedef struct
{
    char *title;
    char *author;
    char *publisher;
    char *date_published;
    int page_count;
} lm_Book;

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

typedef struct
{
    WINDOW *win;
    FIELD **fields;
    FORM *form;
} lm_InsertWindow;

typedef enum
{
    MainWindow, InsertWindow
} lm_ActiveWindow;

typedef struct 
{
    uint8_t is_running;
    lm_ActiveWindow active_win;
    lm_MainWindow *win_main;
    lm_InsertWindow *win_insert;
    MYSQL *database;
} lm_LibState;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~UTILITY FUNCTIONS (util.c)~~~~~~~~~~~~~~ */
size_t              trimwhitespace              (char *out, size_t len, const char *str);
void                lm_event_main               (lm_LibState *state, int input);
void                lm_event_insert             (lm_LibState *state, int input);

/* ~~~~~~~~~~~~~~~~~~~~~~~MAIN WINDOW FUNCTIONS (window_main.c)~~~~~~~~~~~~~~~~~~~~~~ */
lm_MainWindow*      lm_createMainWindow         ();
void                lm_redrawMainWindow         (lm_LibState *state);
void                lm_addRecord                (lm_LibState *state, StringList *record);
void                lm_menu_browse              (lm_LibState *state);
lm_Book*            lm_menu_insert              (lm_LibState *state);
void                lm_menu_remove              (lm_LibState *state);
void                lm_menu_quit                (lm_LibState *state);

/* ~~~~~~~~~~~~~~~~~~~~~~INSERT WINDOW FUNCTIONS (window_insert.c)~~~~~~~~~~~~~~~~~~~~~~ */
lm_InsertWindow*    lm_createInsertWindow       ();
lm_Book*            lm_getBookFields            (lm_InsertWindow *iw);

/* ~~~~~~~~~~~~~~~~~~~~~~~~DATABASE FUNCTIONS (db.c)~~~~~~~~~~~~~~~~~~~~~~ */
void                lm_update_listing           (MYSQL_RES *res, lm_LibState *state);
void                lm_insertBook               (lm_LibState *state, lm_Book *b);

#endif
