#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
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

Book *lm_getBookFields(lm_InsertWindow *iw)
{
    char *field;

    Book *b = (Book*) calloc(1, sizeof(*b));

    for (size_t i = 0; iw->fields[i] != NULL; i++)
    {
        if (i % 2 == 0) continue;

        field = (char*) calloc(1, 255 * sizeof(*field));

        trimwhitespace(field, strlen(field_buffer(iw->fields[i], 0)),
                field_buffer(iw->fields[i], 0));

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
