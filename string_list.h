#ifndef STRING_LIST_INCLUDED
#define STRING_LIST_INCLUDED

typedef struct string_list
{
    char *val;
    struct string_list *next;
} StringList;

void sl_insert(StringList **list, char *s);

#endif
