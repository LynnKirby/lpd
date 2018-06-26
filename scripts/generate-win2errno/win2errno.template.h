/*
# lpd/win2errno.h
> convert Windows system error codes into C standard errno values

Written in 2018 by Lynn Kirby <lynn@arrak.is>

To the extent possible under law, the author has dedicated all copyright and
related and neighboring rights to this software to the public domain worldwide.
This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#ifndef LPD_WIN2ERRNO_H
#define LPD_WIN2ERRNO_H

#ifndef DWORD
#include <stdint.h>
#define DWORD uint32_t
#endif

extern int lpd_win2errno(DWORD err);

#endif

#ifdef LPD_WIN2ERRNO_IMPLEMENTATION

// Note: this section is automatically generated. See
// `scripts/generate-win2errno` in the source tree.

// INSERT HERE

#endif LPD_WIN2ERRNO_IMPLEMENTATION
