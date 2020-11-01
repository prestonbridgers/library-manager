#ifndef LM_H_INCLUDED
#define LM_H_INCLUDED

#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>
#include <mysql.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START LIBRARY HEADER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAIN_MENU_NUM_ENTRIES 5
#define INSERT_FORM_NUM_FIELDS 10
#define WIN_MAIN_NUM_COLUMNS 5

#define INSERT_ITEM 0
#define REMOVE_ITEM 1
#define ABOUT_ITEM  2
#define QUIT_ITEM   3

typedef struct
{
    uint8_t isRunning;

    MYSQL *db;

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

    uint32_t n_records;

    char *content_about;

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

typedef struct
{
    char *title;
    char *author;
    char *publisher;
    char *date_published;
    int page_count;
} db_Book;

LM_STATE*   lm_initState();
void        lm_delState(LM_STATE *s);
void        lm_drawMainWin(LM_STATE *s);
void        lm_drawInsertWin(LM_STATE *s);
void        lm_drawAboutWin(LM_STATE *s);
void        lm_drawRecord(LM_STATE *s, db_Book *book);
db_Book*    lm_handleEvent_insert(LM_STATE *s);
uint8_t     lm_menuItemFunc_insert();
uint8_t     lm_menuItemFunc_remove();
uint8_t     lm_menuItemFunc_about();
uint8_t     lm_menuItemFunc_quit();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START DATABASE HEADER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MYSQL*      db_initDB(char *user, char *pass, char *db_name);
void        db_insertBook(MYSQL *db, char *table, db_Book *book);
void        db_removeBook(MYSQL *db, char *table, char *title);
void        db_destroyBook(db_Book *b);
db_Book*    db_getBookFields(LM_STATE *s);
size_t      trimwhitespace(char *out, size_t len, const char *str);
void        lm_update_listing(MYSQL_RES *res, LM_STATE *state);

#endif
