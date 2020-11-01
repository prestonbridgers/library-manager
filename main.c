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

    LM_STATE *state = lm_initState();
    state->db = db_initDB(username, password, database_name);

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
            case 'q':
                state->isRunning = 0;
                break;
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

    lm_delState(state);
    mysql_library_end();
    endwin();
    return 0;
}
