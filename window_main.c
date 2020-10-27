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
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "library_manager.h"

/*
 * Function creates the instance of lm_Main
 * and returns it.
 *
 * Returns:
 *   A pointer to an initialized instance of lm_MainWindow.
*/
lm_MainWindow *lm_createMainWindow()
{
    lm_MainWindow *mw = (lm_MainWindow *) calloc(1, sizeof(*mw));

    // Vars
    mw->scale = 1.15f;
    uint8_t win_h = LINES / mw->scale;
    uint8_t win_w = COLS / mw->scale;
    uint32_t win_x = (COLS - win_w) / 2;
    uint32_t win_y = (LINES - win_h) / 2;

    // lm_MainWindow* init
    mw->n_records = 0;
    mw->title = "Curt's Library Manager";
    mw->win = newwin(win_h, win_w, win_y, win_x);

    mw->cols[0] = "Title";
    mw->cols[1] = "Page_Count";
    mw->cols[2] = "Publish_Date";
    mw->cols[3] = "Publisher";
    mw->cols[4] = "Author";
    mw->n_cols = sizeof(mw->cols) / sizeof(mw->cols[0]);

    mw->menu_item_list[0] = "Browse";
    mw->menu_item_list[1] = "Insert";
    mw->menu_item_list[2] = "Remove";
    mw->menu_item_list[3] = "Quit";
    mw->menu_item_list[4] = NULL;

    /* lm_MainWindow->menu_items init */
    mw->n_menu_items = sizeof(mw->menu_item_list) / sizeof(mw->menu_item_list[0]);
    mw->menu_items = (ITEM **) calloc(mw->n_menu_items, sizeof(*mw->menu_items));
    for (size_t i = 0; i < mw->n_menu_items; i++)
        mw->menu_items[i] = new_item(mw->menu_item_list[i], NULL);
    mw->menu = new_menu(mw->menu_items);

    set_menu_win(mw->menu, mw->win);
    set_menu_sub(mw->menu, derwin(mw->win, 1, win_w - 2, 3, 1));
    set_menu_format(mw->menu, 1, 4);
    set_menu_mark(mw->menu, " * ");

    set_item_userptr(mw->menu_items[0], &lm_menu_browse);
    set_item_userptr(mw->menu_items[1], &lm_menu_insert);
    set_item_userptr(mw->menu_items[2], &lm_menu_remove);
    set_item_userptr(mw->menu_items[3], &lm_menu_quit);

    return mw;
}

/*
 * Function clears and draws the UI onto a given lm_MainWindow.
 *
 * Paramaters:
 *   lm = The lm_MainWindow to be redrawn (updated).
*/
void lm_redrawMainWindow(lm_LibState *state)
{
    int win_h;
    int win_w;
    getmaxyx(state->win_main->win, win_h, win_w);
    int div_len = win_w / state->win_main->n_cols;

    int starty = 6;
    int startx = 1;

    // Clears the window
    for (int i = 0; i < win_h; i++)
        for (int j = 0; j < win_w; j++)
            mvwaddch(state->win_main->win, i, j, ' ');

    // Boxes the window
    box(state->win_main->win, 0, 0);

    // Draws the title bar
    uint8_t title_w = strlen(state->win_main->title) - 1;
    uint8_t title_x = (win_w - 2 - title_w) / 2;
    uint8_t title_y = 1;
    mvwprintw(state->win_main->win, title_y, title_x, state->win_main->title);
    for (size_t i = 1; i < win_w - 1; i++)
        mvwaddch(state->win_main->win, title_y + 1, i, '-');

    // Draws the columns for the record display area
    for (size_t i = 0; i < state->win_main->n_cols; i++)
    {
        int cur_str_len = strlen(state->win_main->cols[i]);

        if (i != 0)
            wmove(state->win_main->win, starty, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(state->win_main->win, starty, startx + div_len / 2 - cur_str_len / 2);
        wprintw(state->win_main->win, state->win_main->cols[i]);

        // Print column dividers
        for (size_t j = starty; j < win_h - 1; j++)
        {
            if (i == 0) continue;
            wmove(state->win_main->win, j, i * div_len);
            waddch(state->win_main->win, '|');
        }
    }

    // Draws the line beneach the column headers
    for (int i = 1; i < win_w - 1; i++)
        mvwaddch(state->win_main->win, starty + 1, i, '-');

    // Posting menu and refreshing mw->win
    unpost_menu(state->win_main->menu);
    post_menu(state->win_main->menu);
    wrefresh(state->win_main->win);
}

/*
 * Functions draws a properly formatted record onto the main window.
 *
 * Paramaters:
 *   state: Current state of the library manager.
 *   record: Book record as a list of strings to be drawn.
*/
void lm_addRecord(lm_LibState *state, StringList *record)
{
    int num_fields = 0;
    for (StringList *trav = record; trav != NULL; trav = trav->next) num_fields++;

    fprintf(stderr, "In addRecord, num_fields: %d\n", num_fields);

    int win_h;
    int win_w;
    getmaxyx(state->win_main->win, win_h, win_w);
    int div_len = win_w / num_fields;

    int startx = 1;
    int starty = 8;

    // The assignment of i to win_h suppresses a compiler warning
    size_t i = win_h;
    i = 0;
    for (StringList *trav = record; trav != NULL; trav = trav->next)
    {
        int cur_str_len = strlen(trav->val);

        if (i != 0)
            wmove(state->win_main->win, starty + 1 * state->win_main->n_records, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(state->win_main->win, starty + 1 * state->win_main->n_records, startx + div_len / 2 - cur_str_len / 2);
        wprintw(state->win_main->win, trav->val);

        i++;
    }

    wrefresh(state->win_main->win);
    state->win_main->n_records++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TEMPORARY EVENT HANDLING FOR MAIN WINDOW?~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lm_menu_browse(lm_LibState *state)
{
    fprintf(stderr, "User pressed browse\n");
    mvprintw(0, 0, "You pressed browse!");
    refresh();
}

lm_Book *lm_menu_insert(lm_LibState *state)
{
    fprintf(stderr, "User pressed insert\n");
    mvprintw(0, 0, "You pressed insert!");

    // String list of user input to be returned
    lm_Book *book = NULL;

    // Create an InsertWindow to get input from the user
    lm_InsertWindow *iw = lm_createInsertWindow();

    // Make the cursor visible
    curs_set(1);

    // Input loop for the InsertWindow
    int input;
    while ((input = getch()) != 'q')
    {
        switch (input)
        {
            case KEY_DOWN:
            case 9: //Tab
                form_driver(iw->form, REQ_NEXT_FIELD);
                form_driver(iw->form, REQ_END_LINE);
                break;

            case KEY_UP:
                form_driver(iw->form, REQ_PREV_FIELD);
                form_driver(iw->form, REQ_END_LINE);
                break;

            case KEY_LEFT:
                form_driver(iw->form, REQ_PREV_CHAR);
                break;

            case KEY_RIGHT:
                form_driver(iw->form, REQ_NEXT_CHAR);
                break;

                // Delete the char before cursor
            case KEY_BACKSPACE:
            case 127:
                form_driver(iw->form, REQ_DEL_PREV);
                break;

                // Delete the char under the cursor
            case KEY_DC:
                form_driver(iw->form, REQ_DEL_CHAR);
                break;

                // Enter
            case 10:
                form_driver(iw->form, REQ_NEXT_FIELD);
                form_driver(iw->form, REQ_PREV_FIELD);
                form_driver(iw->form, REQ_END_LINE);

                book = lm_getBookFields(iw);
                break;

            default:
                form_driver(iw->form, input);
                break;
        }        
        wrefresh(iw->win);
    }

    unpost_form(iw->form);
    wborder(iw->win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(iw->win);
    delwin(iw->win);
    curs_set(0);

    return book;
}

void lm_menu_remove(lm_LibState *state)
{
    fprintf(stderr, "User pressed remove\n");
    mvprintw(0, 0, "You pressed remove!");
    refresh();
}

void lm_menu_quit(lm_LibState *state)
{
    fprintf(stderr, "User pressed quit\n");
    mvprintw(0, 0, "You pressed quit!  ");
    state->is_running = 0;
    refresh();
}
