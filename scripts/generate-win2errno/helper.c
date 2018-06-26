/*
Written in 2018 by Lynn Kirby <lynn@arrak.is>

To the extent possible under law, the author has dedicated all copyright and
related and neighboring rights to this software to the public domain worldwide.
This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>

#define EINVAL 22

extern int _get_errno_from_oserr(unsigned long);

int main() {
    printf("int lpd_win2errno(DWORD err) {\n");
    printf("    switch(err) {\n");
    printf("    case 0: return 0;\n");

    for (int i = 1; i <= 15999; ++i) {
        int eno = _get_errno_from_oserr(i);

        if (eno != EINVAL) {
            printf("    case %i: return %i;\n", i, eno);
        }
    }

    printf("    default: return 22;\n");
    printf("    }\n");
    printf("}\n");

    return 0;
}

