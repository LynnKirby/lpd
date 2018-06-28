/*
# lpd_termkind.h
> a better `isatty()`

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

#ifndef LPD_TERMKIND_H
#define LPD_TERMKIND_H

#ifdef LPD_TERMKIND_STATIC
#define LPD_TK__DEF static
#else
#define LPD_TK__DEF extern
#endif

#include <stdio.h>

enum lpd_termkind {
    LPD_TK_ERROR,
    LPD_TK_NOTTY,
    LPD_TK_TTY,
    LPD_TK_WIN_PTY,
    LPD_TK_WIN_CONSOLE
};

typedef enum lpd_termkind lpd_termkind;

LPD_TK__DEF lpd_termkind lpd_termkind_stream(FILE* stream);
LPD_TK__DEF lpd_termkind lpd_termkind_file(int fd);

#endif

#ifdef LPD_TERMKIND_IMPLEMENTATION

/*******************************************************************************
 * POSIX implementation.
 */

#ifndef _WIN32

#include <unistd.h>

lpd_termkind lpd_termkind_stream(FILE* stream) {
    int fd = fileno(stream);

    if (fd == -1) {
        return LPD_TK_ERROR;
    }

    return lpd_termkind_file(fd);
}

extern lpd_termkind lpd_termkind_file(int fd) {
    if (isatty(fd) == 1) {
        return LPD_TK_TTY;
    }

    if (errno == EINVAL || errno == ENOTTY) {
        return LPD_TK_NOTTY;
    }

    return LPD_TK_ERROR;
}

#endif

/*******************************************************************************
 * Windows implementation.
 */

#ifdef _WIN32

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

#define LPD_WIN2ERRNO_STATIC
#define LPD_WIN2ERRNO_IMPLEMENTATION
#include "lpd_win2errno.h"

lpd_termkind lpd_termkind_stream(FILE* stream) {
    int fd = _fileno(stream);

    // Invalid FILE* - sets errno.
    if (fd == -1) return LPD_TK_ERROR;

    // Not associated with an output stream - not a TTY.
    if (fd == -2) return LPD_TK_NOTTY;

    return lpd_termkind_file(fd);
}

lpd_termkind lpd_termkind_file(int fd) {
    HANDLE handle = (HANDLE) _get_osfhandle(fd);

    // Invalid HANDLE - sets errno.
    if (handle == INVALID_HANDLE_VALUE) return LPD_TK_ERROR;

    DWORD file_type = GetFileType(handle);

    // If it's a pipe it might be a pseudo-terminal, e.g. mintty.
    if (file_type == FILE_TYPE_PIPE) {
        DWORD size = sizeof(FILE_NAME_INFO) + (sizeof(WCHAR) * MAX_PATH);
        FILE_NAME_INFO* info = (FILE_NAME_INFO* ) malloc(size);
        if (info == NULL) return LPD_TK_ERROR;

        lpd_termkind kind = LPD_TK_NOTTY;

        if (!GetFileInformationByHandleEx(handle, FileNameInfo, info, size)) {
            free(info);
            errno = lpd_win2errno(GetLastError());
            return LPD_TK_ERROR;
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
                    kind = LPD_TK_WIN_PTY;
                }
            }
        }

        free(info);
        return kind;
    }

    // Character device is a Windows TTY.
    if (file_type == FILE_TYPE_CHAR) {
        return LPD_TK_WIN_CONSOLE;
    }

    // If it's something else, it's not a TTY.
    return LPD_TK_NOTTY;
}

#endif

#endif

#ifdef __cplusplus
}
#endif
