#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef enum
{
    MainWindow, InsertWindow
} ActiveWindow;

typedef struct 
{
    uint8_t is_running;
    ActiveWindow active_win;
} LibState;

typedef struct
{
    char *title;
    char *author;
    char *publisher;
    char *date_published;
    int page_count;
}Book;

#endif
