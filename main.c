/*
    Curt's Library Manager, a local SQL database library management system.
    Copyright (C) 2020  Preston Bridgers

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lm.h"

#define EXIT_DB_NOT_FOUND               1
#define EXIT_FAILED_TO_CONNECT_TO_DB    2
#define EXIT_TABLE_NOT_FOUND            3

void clm_terminate(LM_STATE *s, uint8_t exit_code)
{
    endwin();
    switch (exit_code)
    {
        case EXIT_DB_NOT_FOUND:
            fprintf(stderr, "ERROR: DB NOT FOUND\nExiting...\n");
            printf("ERROR: DB NOT FOUND\nExiting...\n");
            break;
        case EXIT_FAILED_TO_CONNECT_TO_DB:
            fprintf(stderr, "ERROR: FAILED TO CONNECT TO TABLE\nExiting...\n");
            printf("ERROR: FAILED TO CONNECT TO TABLE\nExiting...\n");
            break;
        case EXIT_TABLE_NOT_FOUND:
            fprintf(stderr, "ERROR: TABLE NOT FOUND\nExiting...\n");
            printf("ERROR: TABLE NOT FOUND\nExiting...\n");
            break;
        case EXIT_SUCCESS:
            fprintf(stderr, "Exiting: EXIT OK\n");
            printf("Exiting: EXIT OK\n");
            exit(EXIT_SUCCESS);
            break;
    }

    lm_delState(s);
    mysql_library_end();
    exit(EXIT_FAILURE);
}

uint8_t checkDB(MYSQL *db, char *database_name, char *table_name)
{
    uint8_t db_found = 0;
    uint8_t table_found = 0;
    uint8_t err;
    
    MYSQL_RES *res;
    mysql_real_query(db, "SHOW DATABASES", strlen("SHOW DATABASES"));
    res = mysql_use_result(db);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
        if (strcmp(row[0], database_name) == 0)
            db_found = 1;
    mysql_free_result(res);

    if (!db_found) return EXIT_DB_NOT_FOUND;

    char query[255];
    sprintf(query, "USE %s", database_name);
    err = mysql_real_query(db, query, strlen(query));
    if (err) return EXIT_FAILED_TO_CONNECT_TO_DB;

    res = mysql_list_tables(db, NULL);
    while ((row = mysql_fetch_row(res)) != NULL)
        if (strcmp(row[0], table_name) == 0)
            table_found = 1;
    mysql_free_result(res);

    if (!table_found) return EXIT_TABLE_NOT_FOUND;

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n\t%s {database} {table}\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *database_name = argv[1];
    char *table_name    = argv[2];
    char *username = "root";
    char *password = "adamsandler1";

    // DEBUG
    fprintf(stderr, "Program initialization:\n\tDatabase name:\t%s\n\tTable name:\t%s\n\n",
        database_name, table_name);

    initscr();
    curs_set(0);
    noecho();

    //TODO: Change the ordering of these
    LM_STATE *state = lm_initState();
    state->db = db_initDB(username, password, database_name);

    // Checking if there is a database/table with those names
    uint8_t err = checkDB(state->db, database_name, table_name);
    if (err != 0) clm_terminate(state, err);

    PANEL *top = state->pnl_main;
    int32_t input;
    uint8_t isRemove = 0;
    while (state->isRunning)
    {
        lm_drawMainWin(state);
        //start book list update code
        char select_all[255];
        sprintf(select_all, "SELECT * from %s", table_name);
        mysql_real_query(state->db, select_all, strlen(select_all));
        MYSQL_RES *res = mysql_use_result(state->db);
        lm_update_listing(res, state);
        // end db update code
        lm_drawInsertWin(state);
        lm_drawAboutWin(state);
        update_panels();
        doupdate();

        // If current window is the insert window
        if (top == state->pnl_insert)
        {
            db_Book *book = lm_handleEvent_insert(state);

            if (isRemove)
                db_removeBook(state->db, table_name, book->title);
            else
                db_insertBook(state->db, table_name, book);
            isRemove = 0;

            db_destroyBook(book);

            top = state->pnl_main;
            top_panel(top);
            continue;
        }

        input = getch();
        switch (input)
        {
            case 'j':
            case 'h':
                menu_driver(state->menu_main, REQ_LEFT_ITEM);
                break;
            case 'k':
            case 'l':
                menu_driver(state->menu_main, REQ_RIGHT_ITEM);
                break;
            case 10: // Enter
                {
                    ITEM *cur_item = current_item(state->menu_main);
                    uint8_t (*item_func)(void) = item_userptr(cur_item);

                    uint8_t item_selected = item_func();
                    if (item_selected == INSERT_ITEM)
                    {
                        top = (PANEL*) state->pnl_insert;
                        top_panel(top);
                    }
                    else if (item_selected == QUIT_ITEM)
                    {
                        state->isRunning = 0;
                    }
                    else if (item_selected == ABOUT_ITEM)
                    {
                        top = (PANEL*) state->pnl_about;
                        top_panel(top);
                        update_panels();
                        doupdate();
                        getch();
                        top = (PANEL*) state->pnl_main;
                        top_panel(top);
                    }
                    else if (item_selected == REMOVE_ITEM)
                    {
                        top = (PANEL*) state->pnl_insert;
                        top_panel(top);
                        isRemove = 1;
                    }
                }
                break;
        }
    }
    clm_terminate(state, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}
