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
// File Name   : shelltest.cpp
// Description : Shellspawn test harness
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

#include "shellspawn.h"

static char * readline(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax-2;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax - 2;
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

void OutHandle1(char *data, void *context)
{
    printf("OUT(%s)\n", data);
}

void ErrHandle1(char *data, void *context)
{
    printf("ERR(%s)\n", data);
}

int InHandle1(char **data, void *context)
{
    char *response = "repeat\nBilly\n";
    *data = malloc(strlen(response) + 1);
    strcpy(*data,response);
    return 0;
}

int InHandle2(char **data, void *context)
{
    printf("> ");

    *data = readline();

    if (strcmp(*data,"quit") == 0) return 1;
    strcat(*data,"\n");
    return 0;
}

int main(int argc, char **argv) {

    /* Hello */
    printf("Test Harness for shellspawn()\n");

    char *command = "testclient";

    int rc=0;
    char *spawnErrorText = 0;
    int spawnErrorCode;
    int n;
    int i;

    {
        printf("Vector (Array) Test\n");
        STRINGARRAY in = { "Bob Smith", 0};
        STRINGARRAY *out = 0;
        STRINGARRAY *err = 0;
        spawnErrorCode = shellspawn(command, &in, NULL, NULL, NULL,
                                    &out, NULL, NULL, NULL,
                                    &err, NULL, NULL, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);

        // Display stdout
        if (!out || (*out)[0]==0) printf("No Stdout\n");
        else for (i=0; (*out)[i]; i++) printf("Stdout line %d: %s\n", i+1, (*out)[i]);
        if (*out) free(*out);
        // Display stderr
        if (!err || (*err)[0]==0) printf("No Stderr\n");
        else for (i=0; (*err)[i]; i++) printf("Stderr line %d: %s\n", i+1, (*err)[i]);
        if (*err) free(*err);
    }

    {
        printf("\n\nString Test\n");
        char *sIn = "Jones Simon\n";
        char *sOut = 0;
        char *sErr = 0;
        spawnErrorCode = shellspawn(command, NULL, sIn, NULL, NULL,
                                    NULL, &sOut, NULL, NULL,
                                    NULL, &sErr, NULL, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
        //Display stdout
        printf("Stdout: %s\n", sOut);
        if (sOut) free(sOut);
        // Display stderr
        printf("Stderr: %s\n", sErr);
        if (sErr) free(sErr);
    }

    {
        printf("\n\nCall Back Test 1\n");
        spawnErrorCode = shellspawn(command, NULL, NULL, InHandle1, NULL,
                                    NULL, NULL, OutHandle1, NULL,
                                    NULL, NULL, ErrHandle1, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
    }

    {
        printf("\n\nNULL Test\n");
        spawnErrorCode = shellspawn(command, NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
    }

    {
        printf("\n\nCommand does not exist test - should give an error message\n");
        spawnErrorCode = shellspawn("does_not_exist", NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
    }

    {
        char *sOut = 0;
        printf("\n\nCommand does not exist test - should work - arg is hello\n");
        spawnErrorCode = shellspawn("testclient hello", NULL, NULL, NULL, NULL,
                                    NULL, &sOut, NULL, NULL,
                                    NULL, NULL, NULL, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
        printf("Stdout: %s\n", sOut);
    }

    {
        printf("\n\nCall Back Test 2 (interactive - \"quit\" closes stdin)\n");
        spawnErrorCode = shellspawn(command, NULL, NULL, InHandle2, NULL,
                                    NULL, NULL, OutHandle1, NULL,
                                    NULL, NULL, ErrHandle1, NULL, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
    }

    {
        printf("\n\nFILE* Test (see input.txt, output.txt and error.txt)\n");
        FILE* in=fopen("input.txt","r");
        FILE* out=fopen("output.txt","w");
        FILE* err=fopen("error.txt","w");

        spawnErrorCode = shellspawn(command, NULL, NULL, NULL, in,
                                    NULL, NULL, NULL, out,
                                    NULL, NULL, NULL, err, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);

        if (in)
        {
            rc = fclose(in);
            if (rc) printf("Error closing in file\n");
        }
        else printf("Warning input.txt does not exist\n");

        rc = (int)fwrite("Test Harness added this",23,1,out);
        if (rc!=1) printf("Error writing more to out file - rc was not 1 it was %d\n",rc);
        rc = fclose(out);
        if (rc) printf("Error closing out file\n");
        rc = fclose(err);
        if (rc) printf("Error closing err file\n");
    }


    {
        printf("\n\nStdio Test (interactive) - \"quit\" closes stdin)\n");
        spawnErrorCode = shellspawn(command, NULL, NULL, NULL, stdin,
                                    NULL, NULL, NULL, stdout,
                                    NULL, NULL, NULL, stderr, &rc, &spawnErrorText, NULL);
        if (spawnErrorCode) {
            printf("Error Spawning Process. SpawnRC=%d. Error Text=%s\n", spawnErrorCode, spawnErrorText);
            if (spawnErrorText) free(spawnErrorText);
            spawnErrorText = 0;
        }
        printf("RC=%d\n", rc);
    }


    // Loop for Resource leakage test
    // I.e. set it to 10000 and see what happens ....
    {
        printf("\n\nLoop test - look at taskmanger handles/ps/top etc.\n");
        int loop = 100;
        STRINGARRAY in =
                {"repeat","repeat","repeat","repeat","repeat","repeat",
                 "repeat","repeat","repeat","repeat","repeat","repeat",
                 "repeat","repeat","repeat","repeat","Jones Simon",0};
        STRINGARRAY *out = 0;
        STRINGARRAY *err = 0;

        printf("looping %d times (will take a couple of minutes to complete)\n",loop);
        for (n=0; n<loop; n++)
        {
            shellspawn(command, &in, NULL, NULL, NULL,
                                &out, NULL, NULL, NULL,
                                &err, NULL, NULL, NULL, &rc, &spawnErrorText, NULL);
        }

        printf("Done. Press ENTER to exit\n");
        // Note: this must be the last test - so this app ends after this - for the note below to be valid
        printf("Note: Watch handles etc. in taskmanger to see if they drop suddenly (by 1000s) indicating a leek\n");
        fflush(stdout);
        getc(stdin);
    }

    return 0;
}