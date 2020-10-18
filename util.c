#include <ctype.h>
#include <string.h>

#include "util.h"
#include "string_list.h"
#include "ncurses.h"
#include "window_main.h"

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
/*
void event_InsertWindow()
{

}

void event_MainWindow(lm_MainWindow *mWin)
{

    // Input loop
    while (is_running && (input = getch()) != 'q')
    {
        switch (input)
        {
            case 'j':
            case 'h':
                menu_driver(mWin->menu, REQ_LEFT_ITEM);
                break;
            case 'k':
            case 'l':
                menu_driver(mWin->menu, REQ_RIGHT_ITEM);
                break;
            case 10:
                {
                    ITEM *cur = current_item(mWin->menu);
                    void* (*p)(void) = item_userptr(cur);
                    StringList *l = NULL;

                    l = (StringList*) p();
                    if (l == NULL) break;

                    char *field_arr[5];
                    StringList *trav;
                    size_t i = 0;
                    for (trav = l; trav != NULL; trav = trav->next)
                        field_arr[i++] = trav->val;

                    //int pc = atoi(field_arr[0]);
                    char *pubdate = field_arr[1];
                    char *pub = field_arr[2];
                    char *auth = field_arr[3];
                    char *title = field_arr[4];

                    db_addBook(db_con, auth, pub, pubdate, 0, title);

                    break;
                }
        }
        fprintf(stderr, "User pressed: %c (int: %d)\n", (char) input, input); //DEBUG

        mysql_real_query(db_con, query_selectAll, strlen(query_selectAll));
        res = mysql_use_result(db_con);
        lm_redrawMainWindow(mWin);
        update_listing(res, mWin);
        wrefresh(mWin->win);
    }
}*/
