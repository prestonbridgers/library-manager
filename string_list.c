#include <stdio.h>
#include <stdlib.h>

#include "string_list.h"

void sl_insert(StringList **list, char *s)
{
    StringList *node = (StringList*) calloc(1, sizeof(*node));
    node->val = s;
    node->next = *list;

    *list = node;
}

/*
int main(void)
{
    StringList *list = NULL;
    sl_insert(&list, "Curt");
    sl_insert(&list, "Kassy");
    sl_insert(&list, "Jacob");

    for (StringList *trav = list; trav != NULL; trav = trav->next)
        printf("%s\n", trav->val);

    return 0;
}
*/
