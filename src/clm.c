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

/*
 * Function draws given string str at the center of row y
 * onto WINDOW win.
 */
void lm_drawCenteredString(WINDOW *win, uint32_t width, uint32_t y, char *str)
{
    size_t len = strlen(str);
    uint32_t x = (width - len) / 2;

    mvwprintw(win, y, x, str);
}

/*
 * Function draws everything to the main nCurses
 * WINDOW buffer.
 */
void lm_drawMainWin(LM_STATE *s)
{
    // Clearing the entire window
    for (size_t i = 0; i < s->h_main; i++)
        for (size_t j = 0; j < s->w_main; j++)
            mvwaddch(s->win_main, i, j, ' ');

    // Drawing solid border around window
    box(s->win_main, 0, 0);

    // Drawing the title in the top center and the bar underneath
    uint8_t title_y = 1;
    lm_drawCenteredString(s->win_main, s->w_main, title_y, s->title_main);
    for (size_t i = 1; i < s->w_main - 1; i++)
        mvwaddch(s->win_main, title_y + 1, i, '-');

    // Calculating column width
    uint8_t div_len = s->w_main / WIN_MAIN_NUM_COLUMNS;

    // starting x and y position of the columns
    int starty = 6;
    int startx = 1;

    // Draws the columns for the record display area
    for (size_t i = 0; i < WIN_MAIN_NUM_COLUMNS; i++)
    {
        int cur_str_len = strlen(s->win_main_columns[i]);

        if (i != 0)
            wmove(s->win_main, starty, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(s->win_main, starty, startx + div_len / 2 - cur_str_len / 2);
        wprintw(s->win_main, s->win_main_columns[i]);

        // Print column dividers
        for (size_t j = starty; j < s->h_main - 1; j++)
        {
            if (i == 0) continue;
            wmove(s->win_main, j, i * div_len);
            waddch(s->win_main, '|');
        }
    }

    // Draws the line beneach the column headers
    for (int i = 1; i < s->w_main - 1; i++)
        mvwaddch(s->win_main, starty + 1, i, '-');

    // re-posting menu and refreshing mw->win
    unpost_menu(s->menu_main);
    post_menu(s->menu_main);
}

/*
 * Function draws everything to the insert nCurses
 * WINDOW buffer.
 */
void lm_drawInsertWin(LM_STATE *s)
{
    // Clearing the entire window
    for (size_t i = 0; i < s->h_insert; i++)
        for (size_t j = 0; j < s->w_insert; j++)
            mvwaddch(s->win_insert, i, j, ' ');

    // Drawing solid border around window
    box(s->win_insert, 0, 0);

    // Drawing the title in the top center and the bar underneath
    uint8_t title_y = 1;
    lm_drawCenteredString(s->win_insert, s->w_insert, title_y, s->title_insert);
    for (size_t i = 1; i < s->w_insert - 1; i++)
        mvwaddch(s->win_insert, title_y + 1, i, '-');

    unpost_form(s->form_insert);
    post_form(s->form_insert);
}

void lm_drawAboutWin(LM_STATE *s)
{
    // Clearing the entire window
    for (size_t i = 0; i < s->h_about; i++)
        for (size_t j = 0; j < s->w_about; j++)
            mvwaddch(s->win_about, i, j, ' ');

    // Drawing the title in the top center and the bar underneath
    uint8_t title_y = 1;
    lm_drawCenteredString(s->win_about, s->w_about, title_y, s->title_about);
    for (size_t i = 1; i < s->w_about - 1; i++)
        mvwaddch(s->win_about, title_y + 1, i, '-');

    // Draws the content to the about window
    uint8_t content_starty = 4;
    uint8_t x = 1;
    for (char *c = s->content_about; *c != '\0'; c++)
    {
        if (*c == '\n')
        {
            x = 0;
            content_starty++;
        }

        mvwaddch(s->win_about, content_starty, x, *c);
        x++;
    }

    // Drawing "Press any key to continue..." at the bottom
    lm_drawCenteredString(s->win_about, s->w_about, s->h_about - 2,
                          "Press any key to continue...");

    // Drawing solid border around window
    box(s->win_about, 0, 0);
}

/*
 * Function initializes an instance of LM_STATE struct.
 * This struct will be passed all over the program and
 * mutated as required.
 */
LM_STATE *lm_initState(uint8_t sortType)
{
    LM_STATE *local_state = (LM_STATE*) calloc(1, sizeof(*local_state));
    local_state->sortOrder = sortType;
    local_state->isRunning = 1;

    // Initializing main window dimensions and position
    float scale_main = 1.15f;
    local_state->title_main = "Curt's Library Manager";
    local_state->h_main = LINES / scale_main;
    local_state->w_main = COLS / scale_main;
    local_state->x_main = (COLS - local_state->w_main) / 2;
    local_state->y_main = (LINES - local_state->h_main) / 2;
    local_state->n_records = 0;

    // Creating the curses window and creating a panel with it
    local_state->win_main = newwin(local_state->h_main, local_state->w_main,
                                   local_state->y_main, local_state->x_main);

    // Initializing the main window's menu
    local_state->menu_main_itemNames[0] = "Insert";
    local_state->menu_main_itemNames[1] = "Remove";
    local_state->menu_main_itemNames[2] = "About";
    local_state->menu_main_itemNames[3] = "Quit";
    local_state->menu_main_itemNames[4] = NULL;

    local_state->menu_main_items = (ITEM**) calloc(MAIN_MENU_NUM_ENTRIES, sizeof(*local_state->menu_main_items));
    for (size_t i = 0; i < MAIN_MENU_NUM_ENTRIES; i++)
        local_state->menu_main_items[i] = new_item(local_state->menu_main_itemNames[i], NULL);
    local_state->menu_main = new_menu(local_state->menu_main_items);

    set_menu_win(local_state->menu_main, local_state->win_main);
    set_menu_sub(local_state->menu_main, derwin(local_state->win_main, 1, local_state->w_main - 2, 3, 1));
    set_menu_format(local_state->menu_main, 1, 4);
    set_menu_mark(local_state->menu_main, " * ");

    set_item_userptr(local_state->menu_main_items[0], &lm_menuItemFunc_insert);
    set_item_userptr(local_state->menu_main_items[1], &lm_menuItemFunc_remove);
    set_item_userptr(local_state->menu_main_items[2], &lm_menuItemFunc_about);
    set_item_userptr(local_state->menu_main_items[3], &lm_menuItemFunc_quit);

    // Initializing column names for displaying books in the main window
    local_state->win_main_columns[0] = "Title";
    local_state->win_main_columns[1] = "Author";
    local_state->win_main_columns[2] = "Publish_Date";
    local_state->win_main_columns[3] = "Publisher";
    local_state->win_main_columns[4] = "Page_Count";

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIALIZING THE INSERT WINDOW~~~~~~~~~~~~~~~~~
    // Initializing insert window dimensions and position
    float scale_insert = 2.0f;
    local_state->title_insert = "Insert A Book";
    local_state->h_insert = LINES / scale_insert;
    local_state->w_insert = COLS / scale_insert;
    local_state->x_insert = (COLS - local_state->w_insert) / 2;
    local_state->y_insert = (LINES - local_state->h_insert) / 2;

    // Creating the curses window and creating a panel with it
    local_state->win_insert = newwin(local_state->h_insert, local_state->w_insert,
                                   local_state->y_insert, local_state->x_insert);

    local_state->form_insert_fieldNames[0] = "Title";
    local_state->form_insert_fieldNames[1] = "lbl_title";
    local_state->form_insert_fieldNames[2] = "Author";
    local_state->form_insert_fieldNames[3] = "lbl_author";
    local_state->form_insert_fieldNames[4] = "Publisher";
    local_state->form_insert_fieldNames[5] = "lbl_publisher";
    local_state->form_insert_fieldNames[6] = "Publish_Date";
    local_state->form_insert_fieldNames[7] = "lbl_publish_date";
    local_state->form_insert_fieldNames[8] = "Page_Count";
    local_state->form_insert_fieldNames[9] = "lbl_page_count";

    local_state->form_insert_fields = (FIELD**) calloc(INSERT_FORM_NUM_FIELDS + 1, sizeof(*local_state->form_insert_fields));

    int lbl_width = 15;
    int field_width = 40;
    int spacing = 3;
    for (int i = 0; i < INSERT_FORM_NUM_FIELDS; i++)
    {
        if (i % 2 == 0)
        {
            local_state->form_insert_fields[i] = new_field(1, lbl_width, i, 1, 0, 0);
            set_field_buffer(local_state->form_insert_fields[i], 0, local_state->form_insert_fieldNames[i]);
            set_field_opts(local_state->form_insert_fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
        }
        else
        {
            local_state->form_insert_fields[i] = new_field(1, field_width, i - 1, lbl_width + spacing, 0, 0);
            set_field_opts(local_state->form_insert_fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
            set_field_back(local_state->form_insert_fields[i], A_UNDERLINE);
        }
    }
    local_state->form_insert_fields[INSERT_FORM_NUM_FIELDS] = NULL;

    local_state->form_insert = new_form(local_state->form_insert_fields);
    set_form_win(local_state->form_insert, local_state->win_insert);
    set_form_sub(local_state->form_insert, derwin(local_state->win_insert, INSERT_FORM_NUM_FIELDS, lbl_width + field_width + spacing,
                                  (local_state->h_insert - (INSERT_FORM_NUM_FIELDS)) / 2,
                                  (local_state->w_insert - (lbl_width + field_width + spacing)) / 2));
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIALIZING THE ABOUT WINDOW~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    local_state->title_about = "About the Program";
    float scale_about = 2.0f;
    local_state->h_about = LINES / scale_about;
    local_state->w_about = COLS / scale_about;
    local_state->x_about = (COLS - local_state->w_about) / 2;
    local_state->y_about = (LINES - local_state->h_about) / 2;
    local_state->win_about = newwin(local_state->h_about, local_state->w_about,
                                     local_state->y_about, local_state->x_about);

    local_state->content_about = "Curt's Library Manager; an easy to use personal library management system.\nCopyright (C) 2020  Preston Bridgers\n\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.";
    
    local_state->pnl_about = new_panel(local_state->win_about);
    local_state->pnl_insert = new_panel(local_state->win_insert);
    local_state->pnl_main = new_panel(local_state->win_main);
    
    return local_state;
}

void lm_delState(LM_STATE *s)
{
    for (int i = 0; i < MAIN_MENU_NUM_ENTRIES; i++)
        free_item(s->menu_main_items[i]);
    free(s->menu_main_items);
    free_menu(s->menu_main);

    for (int i = 0; i < INSERT_FORM_NUM_FIELDS; i++)
        free_field(s->form_insert_fields[i]);
    free(s->form_insert_fields);
    unpost_form(s->form_insert);
    free_form(s->form_insert);
    delwin(s->win_main);
    delwin(s->win_insert);

    del_panel(s->pnl_main);
    del_panel(s->pnl_insert);

    mysql_close(s->db);

    free(s);
}

/*
 * Functions draws a properly formatted record onto the main window.
 *
 * Paramaters:
 *   state: Current state of the library manager.
 *   record: Book record as a list of strings to be drawn.
*/
void lm_drawRecord(LM_STATE *s, db_Book *book)
{
    int div_len = s->w_main / WIN_MAIN_NUM_COLUMNS;

    int startx = 1;
    int starty = 8;

    for (size_t i = 0; i < WIN_MAIN_NUM_COLUMNS; i++)
    {
        char str[64];
        size_t cur_str_len;

        switch (i)
        {
            case 0:
                sprintf(str, "%s", book->title);
                break;
            case 1:
                sprintf(str, "%s", book->author);
                break;
            case 2:
                sprintf(str, "%s", book->date_published);
                break;
            case 3:
                sprintf(str, "%s", book->publisher);
                break;
            case 4:
                sprintf(str, "%d", book->page_count);
                break;
        }       
        cur_str_len = strlen(str);

        if (i != 0)
            wmove(s->win_main, starty + 1 * s->n_records, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(s->win_main, starty + 1 * s->n_records, startx + div_len / 2 - cur_str_len / 2);

        wprintw(s->win_main, str);
    }

    wrefresh(s->win_main);
    s->n_records++;
}

db_Book *lm_handleEvent_insert(LM_STATE *s)
{
    db_Book *book = NULL;
    int input;
    uint8_t insert_running = 1;

    curs_set(1);

    while (insert_running)
    {
        input = getch();
        switch (input)
        {
            case KEY_DOWN:
            case 9: //Tab
                form_driver(s->form_insert, REQ_NEXT_FIELD);
                form_driver(s->form_insert, REQ_END_LINE);
                break;

            case KEY_UP:
                form_driver(s->form_insert, REQ_PREV_FIELD);
                form_driver(s->form_insert, REQ_END_LINE);
                break;

            case KEY_LEFT:
                form_driver(s->form_insert, REQ_PREV_CHAR);
                break;

            case KEY_RIGHT:
                form_driver(s->form_insert, REQ_NEXT_CHAR);
                break;

                // Delete the char before cursor
            case KEY_BACKSPACE:
            case 127:
                form_driver(s->form_insert, REQ_DEL_PREV);
                break;

                // Delete the char under the cursor
            case KEY_DC:
                form_driver(s->form_insert, REQ_DEL_CHAR);
                break;

                // Enter
            case 10:
                form_driver(s->form_insert, REQ_NEXT_FIELD);
                form_driver(s->form_insert, REQ_PREV_FIELD);
                form_driver(s->form_insert, REQ_END_LINE);

                book = db_getBookFields(s);
                insert_running = 0;
                break;

            default:
                form_driver(s->form_insert, input);
                break;
        }

        wrefresh(s->win_insert);
    }

    curs_set(0);

    // Clearing the fields the user typed
    for (int i = 0; i < INSERT_FORM_NUM_FIELDS; i++)
        if (i % 2 == 1)
            set_field_buffer(s->form_insert_fields[i], 0, "");

    return book;
}

uint8_t lm_menuItemFunc_insert()
{
    return INSERT_ITEM;
}

uint8_t lm_menuItemFunc_remove()
{
    return REMOVE_ITEM;
}

uint8_t lm_menuItemFunc_about()
{
    return ABOUT_ITEM;
}

uint8_t lm_menuItemFunc_quit()
{
    return QUIT_ITEM;
}
