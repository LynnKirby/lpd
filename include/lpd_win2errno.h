/*
# lpd_win2errno.h
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

#ifdef LPD_WIN2ERRNO_STATIC
#define LPD_W2E__DEF static
#else
#define LPD_W2E__DEF extern
#endif

LPD_W2E__DEF int lpd_win2errno(DWORD err);

#endif

#ifdef LPD_WIN2ERRNO_IMPLEMENTATION

// Note: this section is automatically generated. See
// `scripts/generate-win2errno` in the source tree.

int lpd_win2errno(DWORD err) {
    switch(err) {
    case 0: return 0;
    case 2: return 2;
    case 3: return 2;
    case 4: return 24;
    case 5: return 13;
    case 6: return 9;
    case 7: return 12;
    case 8: return 12;
    case 9: return 12;
    case 10: return 7;
    case 11: return 8;
    case 15: return 2;
    case 16: return 13;
    case 17: return 18;
    case 18: return 2;
    case 19: return 13;
    case 20: return 13;
    case 21: return 13;
    case 22: return 13;
    case 23: return 13;
    case 24: return 13;
    case 25: return 13;
    case 26: return 13;
    case 27: return 13;
    case 28: return 13;
    case 29: return 13;
    case 30: return 13;
    case 31: return 13;
    case 32: return 13;
    case 33: return 13;
    case 34: return 13;
    case 35: return 13;
    case 36: return 13;
    case 53: return 2;
    case 65: return 13;
    case 67: return 2;
    case 80: return 17;
    case 82: return 13;
    case 83: return 13;
    case 89: return 11;
    case 108: return 13;
    case 109: return 32;
    case 112: return 28;
    case 114: return 9;
    case 128: return 10;
    case 129: return 10;
    case 130: return 9;
    case 132: return 13;
    case 145: return 41;
    case 158: return 13;
    case 161: return 2;
    case 164: return 11;
    case 167: return 13;
    case 183: return 17;
    case 188: return 8;
    case 189: return 8;
    case 190: return 8;
    case 191: return 8;
    case 192: return 8;
    case 193: return 8;
    case 194: return 8;
    case 195: return 8;
    case 196: return 8;
    case 197: return 8;
    case 198: return 8;
    case 199: return 8;
    case 200: return 8;
    case 201: return 8;
    case 202: return 8;
    case 206: return 2;
    case 215: return 11;
    case 1816: return 12;
    default: return 22;
    }
}

#endif
