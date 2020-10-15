#ifndef WINDOW_INSERT_INCLUDED
#define WINDOW_INSERT_INCLUDED

#include <curses.h>
#include <form.h>

typedef struct
{
    WINDOW *win;
    FIELD **fields;
    FORM *form;
} lm_InsertWindow;

lm_InsertWindow *lm_createInsertWindow();


#endif
