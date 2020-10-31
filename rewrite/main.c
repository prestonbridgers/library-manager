#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

    // Testing book insertion
    fprintf(stderr, "Testing book insertion\n");
    db_Book *b = calloc(1, sizeof(*b));
    b->title            = "George of the Jungle";
    b->author           = "Curt Bridgers";
    b->publisher        = "Bridgers Publishing";
    b->date_published   = "2020";
    b->page_count       = 345;
    db_insertBook(state->db, table_name, b);
    fprintf(stderr, "Inserted book, destroying book struct\n");
    db_destroyBook(b);
    fprintf(stderr, "Destroyed book\n");

    PANEL *top = state->pnl_main;
    int32_t input;
    while (state->isRunning)
    {
        lm_drawMainWin(state);
        lm_drawInsertWin(state);
        update_panels();
        doupdate();

        if (top == state->pnl_insert)
        {
            db_Book *book = lm_handleEvent_insertWindow(state);

            db_insertBook(state->db, table_name, book);
            fprintf(stderr, "Inserted book, destroying book struct\n");
            db_destroyBook(book);
            fprintf(stderr, "Destroyed book\n");

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
            case 9: // Tab
                top = (PANEL*) panel_userptr(top);
                top_panel(top);
                break;
            case 10: // Enter
                {
                    ITEM *cur_item = current_item(state->menu_main);
                    uint8_t (*item_func)(void) = item_userptr(cur_item);

                    if (item_func() == INSERT_WINDOW)
                    {
                        top = (PANEL*) state->pnl_insert;
                        top_panel(top);
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
