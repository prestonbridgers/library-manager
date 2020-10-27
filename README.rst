Curse of the Blind Documentation
================================

Welcome to the documentation for Curse of the Blind (CotB)!

Introduction
============

Curse of the Blind is a project started in 2018 by `Preston Bridgers`_.
The project began as an idea for an old-school style ASCII game.
However, the project is now being pivoted to development as an
ASCII game engine.

.. _preston bridgers: https://www.linkedin.com/in/prestonbridgers

For more detailed documentation please see the `project website`_.

.. _project website: https://student2.cs.appstate.edu/bridgerspc/cotb

Equipment Needed
================

- A linux machine or virtual machine

.. _dl:

Download
========

Dependencies (install these first using your package
manager of choice):

- libncurses-dev

.. note::

	Package names may change depending on the distribution
	of linux you are running. Currently nCurses, the only
	dependency, is extremely common and widely used. The
	above package name is taken from Debian/Ubuntu/Mint
	repositories.

The project can be cloned from its `gitlab page`_ using the
following command:

``git clone https://www.gitlab.com/prestonbridgers/curse-of-the-blind/``

.. _gitlab page: https://www.gitlab.com/prestonbridgers/curse-of-the-blind/

Usage
=====

Quickstart Boilerplate
----------------------

The following is the boilerplate /src/main.c file that comes with
the project :ref:`download <dl>`.

.. code-block:: c
        
        #include "cotb.h"

        #include <stdio.h>
        #include <stdlib.h>
        #include <ncurses.h>

        void event_handle(GAME *g, char input);

        int main(int argc, char *argv[])
        {
	        // vars
	        GAME *game;

	        // Verifying arguments have been passed
	        if (argc < 2)
	        {
		        printf("Usage:\n\t./curse {./map/path}\n");
		        exit(1);
	        }

	        // Initializing CotB
	        game = cotb_init(argv[1]);

	        // GAME LOOP
	        char in;
	        do
	        {
		        map_show(game->map_win);

		        in = getch();

                        /*
                         * This function is also in the
                         * /src/main.c file, but has been
                         * excluded from this example
                         * because it is too long.
                        */
		        event_handle(game, in);

		        if (game->plr->hp <= 0)
			        game->is_running = 0;
	        } while (game->is_running);

	        // Cleanup
	        map_destroy(game->map_win);
	        endwin();
	        return 0;
        }

Compilation and Running
-----------------------

After modifying the source code however you like, compile and run
the game by running the run.sh script located at /src/run.sh

``./run.sh name_of_map.txt``

FAQs
====

This section will be updated with frequently asked questions from both
users and developers. See the :any:`support` section below
for issues not in this section.

.. _support:

Support
=======

For support of any kind, contact me at:

- prestonbridgers@gmail.com

How to Contribute
=================

Developers feel free to clone the repo and submit a pull request with
changes and additions. Please update the documentation to reflect any
proposed changes. No coding style guidelines are in place as of now.

Licensing
=========

MIT License

Copyright (c) 2020 Preston C. Bridgers

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
