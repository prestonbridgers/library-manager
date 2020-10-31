#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include <ctype.h>

#include "lm.h"

/*
 * Initializes a mysql database and returns it.
 */
MYSQL *db_initDB(char *user, char *pass, char *db_name)
{
    MYSQL *db_unconnected;
    MYSQL *db;
    char query[64];
    size_t query_len;
    int err;

    db_unconnected = mysql_init(NULL);
    db = mysql_real_connect(db_unconnected, "localhost", user,
                            pass, NULL, 0, NULL, 0);

    sprintf(query, "USE %s", db_name);
    query_len = strlen(query);

    err = mysql_real_query(db, query, query_len);
    if (err)
        fprintf(stderr, "Error occured using database %s\n", db_name);

    return db;
}

/* db_bookFromRow
 *
 * Fills a db_Book struct from a MYSQL_ROW
 * and returns it.
 */
db_Book *db_bookFromRow(MYSQL_ROW row, int num_fields)
{
    db_Book *l_book = (db_Book*) calloc(1, sizeof(*l_book));
    
    l_book->author          = row[0];
    l_book->publisher       = row[1];
    l_book->date_published  = row[2];
    l_book->page_count      = atoi(row[3]);
    l_book->title           = row[4];

    return l_book;
}

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
void lm_update_listing(MYSQL_RES *res, LM_STATE *state)
{
    MYSQL_ROW row;
    state->n_records = 0;
    int num_fields = mysql_num_fields(res);

    for (; (row = mysql_fetch_row(res)) != NULL;)
    {
        db_Book *book = db_bookFromRow(row, num_fields);

        lm_drawRecord(state, book);
        db_destroyBook(book);
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
void db_insertBook(MYSQL *db, char *table, db_Book *book)
{
    char query[255];
    size_t query_len; 
    int err;
   
    sprintf(query, "INSERT INTO %s VALUES(\"%s\", \"%s\", \"%s\", %d, \"%s\")",
            table, book->author, book->publisher, book->date_published, book->page_count, book->title);
    query_len = strlen(query);

    err = mysql_real_query(db, query, query_len);
    if (err)
        fprintf(stderr, "Error occured adding book\n");
}

/*
 * Destructor for the db_Book struct.
 */
void db_destroyBook(db_Book *b)
{
    free(b);
}

/*
 * Function fills a db_Book struct with the contents
 * of the insert window form and returns it.
 *
 * Returns:
 *   A pointer to a filled db_Book struct.
*/
db_Book *db_getBookFields(LM_STATE *s)
{
    char *field;

    db_Book *b = (db_Book*) calloc(1, sizeof(*b));

    for (size_t i = 0; s->form_insert_fields[i] != NULL; i++)
    {
        if (i % 2 == 0) continue;

        field = (char*) calloc(1, 255 * sizeof(*field));

        trimwhitespace(field, strlen(field_buffer(s->form_insert_fields[i], 0)),
                field_buffer(s->form_insert_fields[i], 0));

        switch (i)
        {
            case 1:
                b->title = field;
                break;
            case 3:
                b->author = field;
                break;
            case 5:
                b->publisher = field;
                break;
            case 7:
                b->date_published = field;
                break;
            case 9:
                b->page_count = atoi(field);
                break;
        }
    }

    return b;
}

/*
 * Function trims all white space from a string.
 *
 * Paramaters:
 *   out = address where the trimmed string will be assigned
 *   len = size of input string
 *   str = input string
 *
 * Returns:
 *   The size of the newly trimmed string.
*/
size_t trimwhitespace(char *out, size_t len, const char *str)
{
    if(len == 0)
        return 0;

    const char *end;
    size_t out_size;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
    {
        *out = 0;
        return 1;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len-1 ? (end - str) : len-1;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}
