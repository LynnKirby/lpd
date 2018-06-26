#define LPD_TERMCOLOR_IMPLEMENTATION
#include "lpd/termcolor.h"

void main() {
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
}