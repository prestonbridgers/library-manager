#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <clm.h>

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
