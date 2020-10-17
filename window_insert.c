#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "window_insert.h"

lm_InsertWindow *lm_createInsertWindow()
{
    lm_InsertWindow *iw = (lm_InsertWindow *) calloc(1, sizeof(*iw));
   
    float scale = 2;
    uint8_t win_h = LINES / scale;
    uint8_t win_w = COLS / scale;
    uint32_t win_x = (COLS - win_w) / 2;
    uint32_t win_y = (LINES - win_h) / 2;
    fprintf(stderr, "x: %d\ny: %d\n", win_x, win_y);
    iw->win = newwin(win_h, win_w, win_y, win_x);
    box(iw->win, 0, 0);
    wrefresh(iw->win);

    char *form_fields[] = {
        "Title",
        "lbl_title",
        "Author",
        "lbl_author",
        "Publisher",
        "lbl_publisher",
        "Publish_Date",
        "lbl_publish_date",
        "Page_Count",
        "lbl_page_count",
    };
    int num_fields = (sizeof(form_fields) / sizeof(form_fields[0]));
    fprintf(stderr, "Num of fields in insert form: %d\n", num_fields); //DEBUG
    iw->fields = (FIELD **) calloc(num_fields + 1, sizeof(*iw->fields));

    int lbl_width = 15;
    int field_width = 40;
    int spacing = 3;
    for (int i = 0; i < num_fields; i++)
    {
        fprintf(stderr, "Adding field: %s\n", form_fields[i]); //DEBUG
        if (i % 2 == 0)
        {
            iw->fields[i] = new_field(1, lbl_width, i, 1, 0, 0);
            set_field_buffer(iw->fields[i], 0, form_fields[i]);
            set_field_opts(iw->fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
        }
        else
        {
            iw->fields[i] = new_field(1, field_width, i - 1, lbl_width + spacing, 0, 0);
            set_field_opts(iw->fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
            set_field_back(iw->fields[i], A_UNDERLINE);
        }
    }
    iw->fields[num_fields] = NULL;

    iw->form = new_form(iw->fields);
    set_form_win(iw->form, iw->win);
    set_form_sub(iw->form, derwin(iw->win, num_fields, lbl_width + field_width + spacing, (win_h - (num_fields)) / 2, (win_w - (lbl_width + field_width + spacing)) / 2));
    post_form(iw->form);
    wrefresh(iw->win);

    return iw;
}

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

StringList *lm_getBookFields(lm_InsertWindow *iw)
{
    StringList *fields_list = NULL;
    char *field;

    for (size_t i = 0; iw->fields[i] != NULL; i++)
    {
        if (i % 2 == 0) continue;

        field = (char*) calloc(1, 255 * sizeof(*field));

        trimwhitespace(field, strlen(field_buffer(iw->fields[i], 0)),
                        field_buffer(iw->fields[i], 0));
        sl_insert(&fields_list, field);
    }

    return fields_list;
}

/*
int main(void)
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(1);
    refresh();

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
                form_driver(iw->form, REQ_DEL_PREV);
                break;

                // Delete the char under the cursor
            case KEY_DC:
                form_driver(iw->form, REQ_DEL_CHAR);
                break;

            default:
                form_driver(iw->form, input);
                break;
        }        

        wrefresh(iw->win);
    }
    
    endwin();
}*/
