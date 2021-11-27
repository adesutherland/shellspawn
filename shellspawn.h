// *************************************************************************
// A B O U T   T H I S   W O R K  -   S H E L L S P A W N
// *************************************************************************
// Work Name   : ShellSpawn
// Description : This provides a simple interface to spawn a process
//             : with redirected input and output
// Copyright   : Copyright (C) 2008 Adrian Sutherland
// *************************************************************************
// A B O U T   T H I S   F I L E
// *************************************************************************
// File Name   : shellspawn.h
// Description : Library header file
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
//             : adrian@open-bpm.org
//             : sutherland@users.sourceforge.net
// Web         : http://rexxobjects.sourceforge.net/
//             : www.open-bpm.org
// Telephone   : Please e-mail for details
// Postal      : UK - Please e-mail for details
// *************************************************************************

#ifndef shellspawn_h
#define shellspawn_h

#include <stdio.h>

// Call back functions for stdout and stderr
//  - data holds the line(s) output by the child process
//  - context is passed from the call to spawnshell()
typedef void(*OUTHANDLER)(char* data, void* context);

// Call back functions for stdin
//  - If this return non-zero then shellspawn() will close the stdin pipe
//    Note: In this case any data put in the data variable is ignored
//  - the function should put input line(s) in the data string
//  - context is passed from the call to spawnshell()
typedef int(*INHANDLER)(char **data, void* context);

// Array of Strings - array null terminated
typedef char *STRINGARRAY[];

// Clear Array of Strings
static void freeTextArray(STRINGARRAY *Array) {
    size_t s;
    if (*Array) {
        for (s=0; (*Array)[s]; s++) free((*Array)[s]);
        free(Array);
    }
}

// Command to spawn the command
//
// - The caller should only populate at most one of vIn, sIn or fIn depending on
//   whether the input will be handled by a vector of strings, a single string, or
//   the call back function. The same applies to Out and Err.
// - A in, out or err method does not need to be specified ... in this case output is
//   discarded, input stream is just closed.
// - Command contains the commands string to execute
// - rc will contain the return code from the command
// - errorText contains a descriptive text of any error in the shellspawn() function
//   (i.e. NOT from the executed child process). This is set if shellspawn() retuns
//   a non-zero return code.
//  - context is passed to the callback handlers
//
// Return codes
//  0 - SHELLSPAWN_OK         - All OK
//  1 - SHELLSPAWN_TOOMANYIN  - More than one of pIn, vIn, sIn or fIn specified.
//  2 - SHELLSPAWN_TOOMANYOUT - More than one of pOut, vOut, sOut or fOut specified.
//  3 - SHELLSPAWN_TOOMANYERR - More than one of pErr, vErr, sErr or fErr specified.
//  4 - SHELLSPAWN_NOFOUND    - The command was not found
//  5 - SHELLSPAWN_FAILURE    - Spawn failed unexpectedly (see error text for details)
int shellspawn(const char *command,
               STRINGARRAY *aIn,
               char* sIn,
               INHANDLER fIn,
               FILE* pIn,
               STRINGARRAY **aOut,
               char** sOut,
               OUTHANDLER fOut,
               FILE* pOut,
               STRINGARRAY **aErr,
               char** sErr,
               OUTHANDLER fErr,
               FILE* pErr,
               int *rc,
               char **errorText,
               void* context);

// Error codes
#define SHELLSPAWN_OK         0
#define SHELLSPAWN_TOOMANYIN  1
#define SHELLSPAWN_TOOMANYOUT 2
#define SHELLSPAWN_TOOMANYERR 3
#define SHELLSPAWN_NOFOUND    4
#define SHELLSPAWN_FAILURE    5

#endif