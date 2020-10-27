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
#include <stdlib.h>

#include "string_list.h"

/*
 * Function inserts a string into the given StringList.
*/
void sl_insert(StringList **list, char *s)
{
    StringList *node = (StringList*) calloc(1, sizeof(*node));
    node->val = s;
    node->next = *list;

    *list = node;
}

/*
 * Function destroys all memory associated with the given
 * String_List.
*/
void sl_free(StringList *list)
{
    if (list == NULL)
        return;

    sl_free(list->next);

    free(list);
}
