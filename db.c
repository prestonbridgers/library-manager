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
#include <string.h>

#include "library_manager.h"

/*
 * Function draws records of a MYSQL_RES to a nCurses window
 * contained in state.
 *
 * Pre-conditions:
 *    res = The result of a SELECT query containing records.
 *
 * Post-conditions:
 *    Records in res will be drawn to the state->main_win->win
 *    nCurses window.
 *
 *    Memory associated with res will be freed.
*/
void lm_update_listing(MYSQL_RES *res, lm_LibState *state)
{
    MYSQL_ROW row;
    StringList *record;
    state->win_main->n_records = 0;
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

        lm_addRecord(state, record);
    }

    mysql_free_result(res);
}

/*
 * Function inserts a book into table book in database.
 *
 * Pre-conditions:
 *   Database in use by state->database contains a table named "book"
 *   which is setup to match the contents of the lm_Book struct.
 *
 * Post-condition:
 *   The contents of the lm_Book struct b will be inserted as a record
 *   in database's book table.
*/
void lm_insertBook(lm_LibState *state, lm_Book *b)
{
    char *table = "book";
    char query[255];
    size_t query_len; 
    int err;
   
    sprintf(query, "INSERT INTO %s VALUES(\"%s\", \"%s\", \"%s\", %d, \"%s\")",
            table, b->author, b->publisher, b->date_published, b->page_count, b->title);
    query_len = strlen(query);

    err = mysql_real_query(state->database, query, query_len);
    if (err)
        fprintf(stderr, "Error occured adding book\n");
}
