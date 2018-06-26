#ifdef _WIN32
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define NTDDI_VERSION 0x06000000
#else
#define _POSIX_C_SOURCE 1
#endif

#define LPD_TERMCOLOR_IMPLEMENTATION
#include "lpd/termcolor.h"

int main() {
    lpd_termcolor_fg(stdout, LPD_TC_RED);
    printf("  RED  ");

    lpd_termcolor_fg(stdout, LPD_TC_GREEN);
    printf("  GRN  ");

    lpd_termcolor_fg(stdout, LPD_TC_BLUE);
    printf("  BLU  ");

    lpd_termcolor_reset(stdout);
    printf("\n");

    lpd_termcolor_bg(stdout, LPD_TC_RED);
    printf("  RED  ");

    lpd_termcolor_bg(stdout, LPD_TC_GREEN);
    printf("  GRN  ");

    lpd_termcolor_bg(stdout, LPD_TC_BLUE);
    printf("  BLU  ");

    lpd_termcolor_reset(stdout);
    printf("\n");

    return 0;
}
