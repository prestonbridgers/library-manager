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
#include <stdlib.h>
#include <string.h>

#include "library_manager.h"

int main(void)
{
    // nCurses init
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();

    fprintf(stderr, "Starting program\n");
    // Initializing library manager state
    lm_LibState lmState;
    lmState.is_running  = 1;
    lmState.active_win  = MainWindow;
    lmState.win_main    = lm_createMainWindow();
    //lmState.win_insert  = lm_createInsertWindow();
    fprintf(stderr, "Initialized library manager state\n");

    // C->MySQL database connector initialization
    MYSQL *db_unconnected;
    MYSQL *db_con;
    MYSQL_RES *res = NULL;
    char *database = "library";
    char *table = "book";
    char *user = "root";
    char *pass = "adamsandler1";

    db_unconnected = mysql_init(NULL);
    db_con = mysql_real_connect(db_unconnected, "localhost", user,
                                       pass, NULL, 0,
                                       NULL, 0);
    lmState.database = db_con;
    fprintf(stderr, "Initialized library manager state database\n");

    // Selecting a database to use
    char use_db[255];
    sprintf(use_db, "USE %s", database);
    mysql_real_query(db_con, use_db, strlen(use_db));

    // Setting up select_all query
    char query_selectAll[255];
    sprintf(query_selectAll, "SELECT * FROM %s", table);

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BEGIN UI CODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    // Initial query for books and update UI's listings
    mysql_real_query(db_con, query_selectAll, strlen(query_selectAll));
    res = mysql_use_result(lmState.database);

    // Input loop
    int input;

    // Initial screen update
    fprintf(stderr, "Calling initial redrawMainWindow\n");
    lm_redrawMainWindow(&lmState);
    fprintf(stderr, "Calling initial update listing\n");
    lm_update_listing(res, &lmState);

    while (lmState.is_running)
    {
        input = getch();
        
        if (input == 'q')
        {
            lmState.is_running = 0;
            continue;
        }

        switch (lmState.active_win)
        {
            case MainWindow:
                lm_event_main(&lmState, input);
                break;
            case InsertWindow:
                lm_event_insert(&lmState, input);
                break;
            default:
                break;
        }

        mysql_real_query(lmState.database, query_selectAll, strlen(query_selectAll));
        res = mysql_use_result(lmState.database);
        lm_redrawMainWindow(&lmState);
        lm_update_listing(res, &lmState);
    }

    // Cleanup code
    delwin(lmState.win_main->win);
    endwin();
    mysql_library_end();
    return 0;
}
