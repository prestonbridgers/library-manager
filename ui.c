#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ncurses.h>
#include <menu.h>

WINDOW *main_window_init();
void menu_browse();
void menu_insert();
void menu_remove();
void menu_quit();

uint8_t is_running = 1;

MENU *menu;
WINDOW *mainw;

int main()
{
    // nCurses init
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();

    // Vars
    //WINDOW *mainw = main_window_init();
    mainw = main_window_init();
    int input;

    // Input loop
    while (is_running && (input = getch()) != 'q')
    {
        switch (input)
        {
            case 'j':
            case 'h':
                menu_driver(menu, REQ_LEFT_ITEM);
                break;
            case 'k':
            case 'l':
                menu_driver(menu, REQ_RIGHT_ITEM);
                break;
            case 10: /* Enter */
            {
                ITEM *cur = current_item(menu);
                void (*p)(void) = item_userptr(cur);
                p();
                break;
            }
        }

        wrefresh(mainw);
    }

    // cleanup
    delwin(mainw);
    endwin();
    return 0;
}

WINDOW *main_window_init()
{
    /* WINDOW VARS */
    WINDOW *tmp;
    float scale = 1.15f;
    uint8_t win_h = LINES / scale;
    uint8_t win_w = COLS / scale;
    uint32_t win_x = (COLS - win_w) / 2;
    uint32_t win_y = (LINES - win_h) / 2;

    /* TITLE VARS */
    char *title = "Curt's Library Manager";
    uint8_t title_w = strlen(title) - 1;
    uint8_t title_x = (win_w - 2 - title_w) / 2;
    uint8_t title_y = 1;

    /* MENU VARS */
    char *menu_choices[] = {"Browse",
                            "Insert",
                            "Remove",
                            "Quit",
                            NULL     };
    uint8_t menu_choices_len = sizeof(menu_choices) / sizeof(menu_choices[0]);
    ITEM **menu_items = (ITEM **) calloc(menu_choices_len, sizeof(ITEM *));
    //Menu *menu;

    // Creating the window
    tmp = newwin(win_h, win_w, win_y, win_x);

    // Initializing the list of ITEMs
    for (size_t i = 0; i < menu_choices_len; i++)
        menu_items[i] = new_item(menu_choices[i], NULL);

    // Setting the userptr for each item
    set_item_userptr(menu_items[0], &menu_browse);
    set_item_userptr(menu_items[1], &menu_insert);
    set_item_userptr(menu_items[2], &menu_remove);
    set_item_userptr(menu_items[3], &menu_quit);

    // Creating the menu w/ the list of items initalized above
    menu = new_menu(menu_items);

    // Setting the menu's parent window
    set_menu_win(menu, tmp);
    set_menu_sub(menu, derwin(tmp, 1, win_w - 2, 3, 1));
    set_menu_format(menu, 1, 4);

    // Drawing
    box(tmp, 0, 0);
    mvwprintw(tmp, title_y, title_x, title);
    for (size_t i = 1; i < win_w - 1; i++)
        mvwaddch(tmp, title_y + 1, i, '-');

    // Setting menu mark and posting menu and refreshing windows
    set_menu_mark(menu, " * ");
    post_menu(menu);
    wrefresh(tmp);
    refresh();

    return tmp;
}

void menu_browse()
{
    mvprintw(0, 0, "You pressed browse!");
    refresh();
}

void menu_insert()
{
    mvprintw(0, 0, "You pressed insert!");
    refresh();
}

void menu_remove()
{
    mvprintw(0, 0, "You pressed remove!");
    refresh();
}

void menu_quit()
{
    mvprintw(0, 0, "You pressed quit!  ");
    is_running = 0;
    refresh();
}
