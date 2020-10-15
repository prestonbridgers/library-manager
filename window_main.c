#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "string_list.h"
#include "window_insert.h"
#include "window_main.h"

extern int is_running;

lm_MainWindow *lm_createMainWindow()
{
    /* Memory Alloc */
    lm_MainWindow *mw = (lm_MainWindow *) calloc(1, sizeof(*mw));
    mw->num_records = 0;

    /* WINDOW init */
    float scale = 1.15f;
    uint8_t win_h = LINES / scale;
    uint8_t win_w = COLS / scale;
    uint32_t win_x = (COLS - win_w) / 2;
    uint32_t win_y = (LINES - win_h) / 2;

    char *title = "Curt's Library Manager";
    uint8_t title_w = strlen(title) - 1;
    uint8_t title_x = (win_w - 2 - title_w) / 2;
    uint8_t title_y = 1;

    mw->win = newwin(win_h, win_w, win_y, win_x);

    box(mw->win, 0, 0);
    mvwprintw(mw->win, title_y, title_x, title);
    for (size_t i = 1; i < win_w - 1; i++)
        mvwaddch(mw->win, title_y + 1, i, '-');

    /* lm_MainWindow->menu_items init */
    char *menu_item_list[] = {
        "Browse",
        "Insert",
        "Remove",
        "Quit",
        NULL
    };
    uint8_t n_menu_items = sizeof(menu_item_list) / sizeof(menu_item_list[0]);
    mw->menu_items = (ITEM **) calloc(n_menu_items, sizeof(*(mw->menu_items)));
    for (size_t i = 0; i < n_menu_items; i++)
        mw->menu_items[i] = new_item(menu_item_list[i], NULL);
    mw->menu = new_menu(mw->menu_items);
    // Setting lm->menu options
    set_menu_win(mw->menu, mw->win);
    set_menu_sub(mw->menu, derwin(mw->win, 1, win_w - 2, 3, 1));
    set_menu_format(mw->menu, 1, 4);
    set_menu_mark(mw->menu, " * ");
    // Posting menu and refreshing mw->win
    post_menu(mw->menu);
    wrefresh(mw->win);

    //Setting the userptr for each item
    set_item_userptr(mw->menu_items[0], &lm_menu_browse);
    set_item_userptr(mw->menu_items[1], &lm_menu_insert);
    set_item_userptr(mw->menu_items[2], &lm_menu_remove);
    set_item_userptr(mw->menu_items[3], &lm_menu_quit);

    char *cols[] = {
        "Title",
        "Page_Count",
        "Publish_Date",
        "Publisher",
        "Author",
    };
    int cols_len = sizeof(cols) / sizeof(cols[0]);

    lm_fillMainWindowColumns(mw, 6, 1, cols, cols_len);

    return mw;
}

void lm_fillMainWindowColumns(lm_MainWindow *lm, uint8_t starty, uint8_t startx, char *data[], int columns)
{
    int win_h;
    int win_w;
    getmaxyx(lm->win, win_h, win_w);
    int div_len = win_w / columns;

    for (size_t i = 0; i < columns; i++)
    {
        int cur_str_len = strlen(data[i]);

        if (i != 0)
            wmove(lm->win, starty, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(lm->win, starty, startx + div_len / 2 - cur_str_len / 2);
        wprintw(lm->win, data[i]);

        // Print dividers
        for (size_t j = starty; j < win_h - 1; j++)
        {
            if (i == 0) continue;
            wmove(lm->win, j, i * div_len);
            waddch(lm->win, '|');
        }
    }

    for (int i = 1; i < win_w - 1; i++)
    {
        wmove(lm->win, starty + 1, i);
        waddch(lm->win, '-');
    }

    wrefresh(lm->win);
}

void lm_addRecord(lm_MainWindow *mw, StringList *record)
{
    int num_fields = 0;
    for (StringList *trav = record; trav != NULL; trav = trav->next) num_fields++;

    int win_h;
    int win_w;
    getmaxyx(mw->win, win_h, win_w);
    int div_len = win_w / num_fields;

    int startx = 1;
    int starty = 8;

    size_t i = 0;
    for (StringList *trav = record; trav != NULL; trav = trav->next)
    {
        int cur_str_len = strlen(trav->val);

        if (i != 0)
            wmove(mw->win, starty + 1 * mw->num_records, startx + i * div_len - cur_str_len / 2 + div_len/2);
        else
            wmove(mw->win, starty + 1 * mw->num_records, startx + div_len / 2 - cur_str_len / 2);
        wprintw(mw->win, trav->val);

        i++;
    }

    wrefresh(mw->win);
    mw->num_records++;
}

void lm_menu_browse()
{
    fprintf(stderr, "User pressed browse\n");
    mvprintw(0, 0, "You pressed browse!");
    refresh();
}

void lm_menu_insert()
{
    fprintf(stderr, "User pressed insert\n");
    mvprintw(0, 0, "You pressed insert!");

    lm_InsertWindow *iw = lm_createInsertWindow();
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

            case 10:
                form_driver(iw->form, REQ_NEXT_FIELD);
                form_driver(iw->form, REQ_PREV_FIELD);
                fprintf(stderr, "\n\n");
                for (size_t i = 0; iw->fields[i] != NULL; i++)
                    if (i % 2 == 1)
                        fprintf(stderr, "Field #%ld: %s\n", i, field_buffer(iw->fields[i], 0));
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
}

void lm_menu_remove()
{
    fprintf(stderr, "User pressed remove\n");
    mvprintw(0, 0, "You pressed remove!");
    refresh();
}

void lm_menu_quit()
{
    fprintf(stderr, "User pressed quit\n");
    mvprintw(0, 0, "You pressed quit!  ");
    is_running = 0;
    refresh();
}
