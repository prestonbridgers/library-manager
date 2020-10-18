#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>

#include "string_list.h"
#include "window_main.h"
#include "structs.h"

int is_running = 1;

void update_listing(MYSQL_RES *res, lm_MainWindow *mWin)
{
    MYSQL_ROW row;
    StringList *record;
    mWin->n_records = 0;
    int num_fields = mysql_num_fields(res);

    for (; (row = mysql_fetch_row(res)) != NULL;)
    {
        record = NULL;
        for (int i = 0; i < num_fields; i++)
        {
            fprintf(stderr, "Adding record: %s\n", row[i]); //DEBUG
            sl_insert(&record, row[i]);
        }
        fprintf(stderr, "\n"); //DEBUG

        lm_addRecord(mWin, record);
    }
}

void db_addBook(MYSQL *db_con, Book *b)
{
    char *table = "book";
    char query[255];
    size_t query_len; 
    int err;
   
    sprintf(query, "INSERT INTO %s VALUES(\"%s\", \"%s\", \"%s\", %d, \"%s\")",
            table, b->author, b->publisher, b->date_published, b->page_count, b->title);
    query_len = strlen(query);

    err = mysql_real_query(db_con, query, query_len);
    if (err)
        fprintf(stderr, "Error occured adding book\n");
}

int main(void)
{
    // Vars
    MYSQL *db;
    MYSQL_RES *res = NULL;
    char *database = "library";
    char *table = "book";
    char *user = "root";
    char *pass = "adamsandler1";

    LibState lib;
    lib.is_running = 1;
    lib.active_win = MainWindow;
    
    // Initializing a new mysql struct
    db = mysql_init(NULL);

    // Connecting to the MySQL server running on localhost
    MYSQL *db_con = mysql_real_connect(db, "localhost", user,
                                       pass, NULL, 0,
                                       NULL, 0);

    // Selecting a database to use
    char use_db[255];
    sprintf(use_db, "USE %s", database);
    mysql_real_query(db_con, use_db, strlen(use_db));

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BEGIN UI CODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();

    // VARS
    lm_MainWindow *mWin = lm_createMainWindow();
    int input;

    char query_selectAll[255];
    sprintf(query_selectAll, "SELECT * FROM %s", table);

    // Initial query for books and update UI's listings
    mysql_real_query(db_con, query_selectAll, strlen(query_selectAll));
    res = mysql_use_result(db_con);
    update_listing(res, mWin);

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
                void* (*p)(void) = item_userptr(cur);

                Book *b = (Book*) calloc(1, sizeof(*b));
                b = (Book*) p();
                if (b == NULL) break;

                db_addBook(db_con, b);

                break;
            }
        }
        fprintf(stderr, "User pressed: %c (int: %d)\n", (char) input, input); //DEBUG
                
        mysql_real_query(db_con, query_selectAll, strlen(query_selectAll));
        res = mysql_use_result(db_con);
        lm_redrawMainWindow(mWin);
        update_listing(res, mWin);
        wrefresh(mWin->win);
    }

    delwin(mWin->win);
    endwin();
    mysql_library_end();
    return 0;
}
