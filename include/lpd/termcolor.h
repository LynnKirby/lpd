/*
# lpd/termcolor.h
> portable terminal colors

Written in 2018 by Lynn Kirby <lynn@arrak.is>

To the extent possible under law, the author has dedicated all copyright and
related and neighboring rights to this software to the public domain worldwide.
This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LPD_TERMCOLOR_H
#define LPD_TERMCOLOR_H

// Compiling with MSVC and /Wall shows a bunch of errors in the Windows includes
// so just disable the errors temporarily.
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#include <assert.h>
#include <stdio.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

enum lpd_tc_color {
    LPD_TC_UNSET_COLOR = ~1,
    LPD_TC_DEFAULT_COLOR = ~0,
    LPD_TC_BLACK = 0,
    LPD_TC_BLUE = 1 << 0,
    LPD_TC_GREEN = 1 << 1,
    LPD_TC_RED = 1 << 2,
    LPD_TC_CYAN = LPD_TC_GREEN | LPD_TC_BLUE,
    LPD_TC_MAGENTA = LPD_TC_RED | LPD_TC_BLUE,
    LPD_TC_YELLOW = LPD_TC_RED | LPD_TC_GREEN,
    LPD_TC_WHITE = LPD_TC_RED | LPD_TC_GREEN | LPD_TC_BLUE
};

enum lpd_tc_result {
    LPD_TC_OK = 0,
    LPD_TC_EFILE = 1,
    LPD_TC_EERRNO = 2,
    LPD_TC_EWIN = 3
};

typedef enum lpd_tc_color lpd_tc_color;
typedef enum lpd_tc_result lpd_tc_result;

extern lpd_tc_result lpd_termcolor(FILE* stream, lpd_tc_color fg, lpd_tc_color bg);
extern lpd_tc_result lpd_termcolor_fg(FILE* stream, lpd_tc_color color);
extern lpd_tc_result lpd_termcolor_bg(FILE* stream, lpd_tc_color color);
extern lpd_tc_result lpd_termcolor_reset(FILE* stream);

#endif

#ifdef LPD_TERMCOLOR_IMPLEMENTATION

/*******************************************************************************
 * Implementation of simple functions. Just pass off to `lpd_termcolor()`.
 */

lpd_tc_result lpd_termcolor_fg(FILE* stream, lpd_tc_color color) {
    return lpd_termcolor(
        stream,
        color,
        LPD_TC_UNSET_COLOR);
}

lpd_tc_result lpd_termcolor_bg(FILE* stream, lpd_tc_color color) {
    return lpd_termcolor(
        stream,
        LPD_TC_UNSET_COLOR,
        color);
}

lpd_tc_result lpd_termcolor_reset(FILE* stream) {
    return lpd_termcolor(
        stream,
        LPD_TC_DEFAULT_COLOR,
        LPD_TC_DEFAULT_COLOR);
}

/*******************************************************************************
 * Generic ANSI escape code implementation.
 */

static lpd_tc_result lpd_tc__ansi(FILE* stream, lpd_tc_color fg, lpd_tc_color bg) {
    int retval = 0;

    switch (fg) {
    case LPD_TC_BLACK:          retval = fputs("\x1b[30m", stream); break;
    case LPD_TC_RED:            retval = fputs("\x1b[31m", stream); break;
    case LPD_TC_GREEN:          retval = fputs("\x1b[32m", stream); break;
    case LPD_TC_YELLOW:         retval = fputs("\x1b[33m", stream); break;
    case LPD_TC_BLUE:           retval = fputs("\x1b[34m", stream); break;
    case LPD_TC_MAGENTA:        retval = fputs("\x1b[35m", stream); break;
    case LPD_TC_CYAN:           retval = fputs("\x1b[36m", stream); break;
    case LPD_TC_WHITE:          retval = fputs("\x1b[37m", stream); break;
    case LPD_TC_DEFAULT_COLOR:  retval = fputs("\x1b[39m", stream); break;
    case LPD_TC_UNSET_COLOR:    /* do nothing */ break;
    }

    if (retval == EOF) return LPD_TC_EFILE;

    switch (bg) {
    case LPD_TC_BLACK:          retval = fputs("\x1b[40m", stream); break;
    case LPD_TC_RED:            retval = fputs("\x1b[41m", stream); break;
    case LPD_TC_GREEN:          retval = fputs("\x1b[42m", stream); break;
    case LPD_TC_YELLOW:         retval = fputs("\x1b[43m", stream); break;
    case LPD_TC_BLUE:           retval = fputs("\x1b[44m", stream); break;
    case LPD_TC_MAGENTA:        retval = fputs("\x1b[45m", stream); break;
    case LPD_TC_CYAN:           retval = fputs("\x1b[46m", stream); break;
    case LPD_TC_WHITE:          retval = fputs("\x1b[47m", stream); break;
    case LPD_TC_DEFAULT_COLOR:  retval = fputs("\x1b[49m", stream); break;
    case LPD_TC_UNSET_COLOR:    /* do nothing */ break;
    }

    if (retval == EOF) return LPD_TC_EFILE;

    return LPD_TC_OK;
}

/*******************************************************************************
 * POSIX implementatation.
 */

#ifndef _WIN32

#include <errno.h>
#include <unistd.h>

lpd_tc_result lpd_termcolor(FILE* stream, lpd_tc_color fg, lpd_tc_color bg) {
    int fd = fileno(stream);
    if (fd < 0) return LPD_TC_EERRNO;

    if (!isatty(fd)) {
        if (errno == ENOTTY) return LPD_TC_OK;
        return LPD_TC_EERRNO;
    }

    return lpd_tc__ansi(stream, fg, bg);
}

#endif

/*******************************************************************************
 * Windows implementation.
 */

#ifdef _WIN32

#define LPD_TC__USE_NOTTY 0
#define LPD_TC__USE_ANSI 1
#define LPD_TC__USE_WINAPI 2

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <Windows.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

static lpd_tc_result lpd_tc__stream_type(FILE* stream, int* type, HANDLE* handle) {
    // 1. Get the file descriptor.
    int fd = _fileno(stream);

    // Invalid FILE* - sets errno.
    if (fd == -1) return LPD_TC_EERRNO;

    // Not associated with an output stream - not a TTY.
    if (fd == -2) {
        *type = LPD_TC__USE_NOTTY;
        return LPD_TC_OK;
    }

    // 2. Get the OS handle.
    *handle = (HANDLE) _get_osfhandle(fd);

    // Invalid HANDLE - sets errno.
    if (*handle == INVALID_HANDLE_VALUE) return LPD_TC_EERRNO;

    // 3. Check the type of the HANDLE.
    DWORD file_type = GetFileType(*handle);

    // If it's a pipe it might be a pseudo-terminal, e.g. mintty.
    if (file_type == FILE_TYPE_PIPE) {
        *type = LPD_TC__USE_NOTTY;

        DWORD size = sizeof(FILE_NAME_INFO) + (sizeof(WCHAR) * MAX_PATH);
        FILE_NAME_INFO* info = malloc(size);
        if (info == NULL) return LPD_TC_EERRNO;

        if (!GetFileInformationByHandleEx(*handle, FileNameInfo, info, size)) {
            free(info);
            return LPD_TC_EWIN;
        }

        if (info->FileNameLength != 0) {
            // Check if the filename starts with \cygwin- or \msys-
            // and contains both -pty and -master.
            // TODO: Parse the actual format.
            if (wcsncmp(L"\\cygwin-", info->FileName, wcslen(L"\\cygwin-")) == 0 ||
                wcsncmp(L"\\msys-", info->FileName, wcslen(L"\\msys-")) == 0
            ) {
                if (wcsstr(info->FileName, L"-pty") != NULL &&
                    wcsstr(info->FileName, L"-master") != NULL
                ) {
                    *type = LPD_TC__USE_ANSI;
                }
            }
        }

        free(info);
        return LPD_TC_OK;
    }

    // Character device is a Windows TTY.
    if (file_type == FILE_TYPE_CHAR) {
        *type = LPD_TC__USE_WINAPI;
        return LPD_TC_OK;
    }

    // If it's something else, it's not a TTY.
    *type = LPD_TC__USE_NOTTY;
    return LPD_TC_OK;
}

lpd_tc_result lpd_tc__winapi(HANDLE handle, lpd_tc_color fg, lpd_tc_color bg) {
    // If we want to retain any colors then we need to query for them.
    if (fg == LPD_TC_UNSET_COLOR || bg == LPD_TC_UNSET_COLOR) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(handle, &info)) return LPD_TC_EWIN;

        if (fg == LPD_TC_UNSET_COLOR) {
            fg = (lpd_tc_color) (info.wAttributes & LPD_TC_WHITE);
        }

        if (bg == LPD_TC_UNSET_COLOR) {
            bg = (lpd_tc_color) ((info.wAttributes >> 4) & LPD_TC_WHITE);
        }
    }

    // There is no default color on Windows, so just use white text on black.
    if (fg == LPD_TC_DEFAULT_COLOR) fg = LPD_TC_WHITE;
    if (bg == LPD_TC_DEFAULT_COLOR) bg = LPD_TC_BLACK;

    WORD attributes = (WORD) ((fg) | (bg << 4));

    // Blue foreground is unreadable so use increased intensity.
    // (Even with intense blue it's hard to read...)
    if (fg == LPD_TC_BLUE) {
        attributes |= FOREGROUND_INTENSITY;
    }

    if (SetConsoleTextAttribute(handle, attributes)) return LPD_TC_OK;
    return LPD_TC_EWIN;
}

lpd_tc_result lpd_termcolor(FILE* stream, lpd_tc_color fg, lpd_tc_color bg) {
    int type;
    HANDLE handle;

    lpd_tc_result result = lpd_tc__stream_type(stream, &type, &handle);
    if (result != LPD_TC_OK) return result;

    if (type == LPD_TC__USE_WINAPI) {
        return lpd_tc__winapi(handle, fg, bg);
    }

    if (type == LPD_TC__USE_ANSI) {
        return lpd_tc__ansi(stream, fg, bg);
    }

    // Not a TTY, do nothing.
    return LPD_TC_OK;
}

#undef LPD_TC__USE_NOTTY
#undef LPD_TC__USE_WINAPI
#undef LPD_TC__USE_ANSI

#endif
#endif

#ifdef __cplusplus
}
#endif
