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

#include <ctype.h>
#include <string.h>
#include <ncurses.h>

#include "library_manager.h"

/*
 * Function trims all white space from a string.
 *
 * Paramaters:
 *   out = address where the trimmed string will be assigned
 *   len = size of input string
 *   str = input string
 *
 * Returns:
 *   The size of the newly trimmed string.
*/
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
 * Functions handles input for the main window of the
 * library manager.
 *
 * Paramaters:
 *   state: Current state of the library manager.
 *   input: The input to be handled.
*/
void lm_event_main(lm_LibState *state, int input)
{
    switch (input)
    {
        case 'j':
        case 'h':
            menu_driver(state->win_main->menu, REQ_LEFT_ITEM);
            break;
        case 'k':
        case 'l':
            menu_driver(state->win_main->menu, REQ_RIGHT_ITEM);
            break;
        case 10: // Enter
            {
                ITEM *cur = current_item(state->win_main->menu);
                void* (*p)(lm_LibState*) = item_userptr(cur);

                lm_Book *b = (lm_Book*) calloc(1, sizeof(*b));
                b = (lm_Book*) p(state);
                if (b == NULL) break;

                lm_insertBook(state, b);

                break;
            }
    }
}

/*
 * Functions handles input for the insert window of
 * the library manager.
 *
 * Paramaters:
 *   state: Current state of the library manager.
 *   input: The input to be handled.
*/
void lm_event_insert(lm_LibState *state, int input)
{
}
