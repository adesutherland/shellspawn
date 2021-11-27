// *************************************************************************
// A B O U T   T H I S   W O R K  -   S H E L L S P A W N
// *************************************************************************
// Work Name   : ShellSpawn
// Description : This provides a simple interface to spawn a process
//             : with redirected input and output
// Copyright   : Copyright (C) 2008,2021 Adrian Sutherland
// *************************************************************************
// A B O U T   T H I S   F I L E
// *************************************************************************
// File Name   : testclient.c
// Description : Test client for shellspawn (C)
// *************************************************************************
// L I C E N S E
// *************************************************************************
// This program is free software: you can redistribute it and/or modify
// it under the terms of version 3 of the GNU General Public License as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// For the avoidance of doubt:
// - Version 3 of the license (i.e. not earlier nor later versions) apply.
// - a copy of the license text should be in the "license" directory of the
//   source distribution.
// - Requests for use under other licenses will be treated sympathetically,
//   please see contact details.
// *************************************************************************
// C O N T A C T   D E T A I L S
// *************************************************************************
// E-mail      : adrian@sutherlandonline.org
// *************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char * readline(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax-2;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF) {
            if (ferror(stdin) && errno == EINTR) continue;
            break;
        }

        if(--len == 0) {
            len = lenmax-2;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((char)c == '\n')
            break;

        *line++ = (char)c;
    }
    *line = '\0';
    return linep;
}

int main(int argc, char **argv)
{
    int i;

    /* Hello */
    printf("Test Client for AVShell\n");
    fflush(stdout);

    /* Test Args */
    if (argc==0) printf("No arguments\n");
    else for (i=0; i<argc; i++)
        printf("Argument %d:%s\n",i,argv[i]);
    fflush(stdout);

    /* Test stderr */
    fprintf(stderr, "This is an error message\n");
    fflush(stderr);
    /* Test stdin */
    char *name;

    while (1) {
        printf("What is your name?\n");
        fflush(stdout);
        name = readline();
        if (strcmp("repeat",name) != 0)  break;
        printf("Please repeat that!\n");
        free(name);
        fflush(stdout);
    }
    printf("Your name is %s\n",name);
    fflush(stdout);
    free(name);

    /* Test stderr */
    fprintf(stderr, "This is another error message\n");
    fflush(stderr);

    return 123;
}