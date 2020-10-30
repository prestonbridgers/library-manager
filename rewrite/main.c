#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "clm.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n\t%s {database} {table}\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *database_name = argv[1];
    char *table_name    = argv[2];

    // DEBUG
    fprintf(stderr, "Program initialization:\n\tDatabase name:\t%s\n\tTable name:\t%s\n\n",
        database_name, table_name);

    initscr();
    curs_set(0);
    noecho();

    LM_STATE *state = lm_initState();

    PANEL *top = state->pnl_insert;
    int32_t input;
    while (state->isRunning)
    {
        lm_drawMainWin(state);
        lm_drawInsertWin(state);
        update_panels();
        doupdate();

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
            case 9:
                top = (PANEL*) panel_userptr(top);
                top_panel(top);
                break;
            case 10: // Enter
                break;
        }
    }

    endwin();
    return 0;
}
