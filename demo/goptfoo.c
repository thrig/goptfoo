/***********************************************************************
 * Example script for goptfoo(3) that shows how its various functions
 * can be integrated with getopt(3). Note that the auto* tools will
 * install a shell wrapper that is then difficult to debug; try
 *
 *   gcc `pkg-config --libs --cflags goptfoo` -g -std=c99 -o gof goptfoo.c
 *
 * to build something that can be passed to the debugger. This does
 * require that the library be installed.
 */

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

/* A program using the installed version would instead use <goptfoo.h>
 * and `pkg-config` to obtain the necessary compile flags. */
#include "goptfoo.h"

#define MAX_DOUBLES 9UL
#define MAX_LOLLS 33UL
#define MAX_ULOLS 1UL

void emit_help(void);

int main(int argc, char *argv[])
{
    int ch;
    double *thedoubles = NULL;
    long long *thelolls = NULL;
    unsigned long *theulols = NULL;
    size_t numdoubles, numlls, numuls;

    // pre-population is supported, for beter or worse
    if ((thelolls = malloc((size_t) 7 * sizeof(long long))) == NULL)
        err(EX_OSERR, "could not allocate list of lolls");
    for (size_t i = 0; i < 7; i++) {
        thelolls[i] = 100 + i;
    }
    /* numdoubles is not passed in unlike these two, so need not be
     * set. However, it is probably safer to do so, in the event
     * something changes. */
    numlls = 7;
    numuls = 0;

    while ((ch = getopt(argc, argv, "h?d:f:l:u:D:L:U:")) != -1) {
        switch (ch) {
        case 'd':
            printf("%.2f\n", flagtod(ch, optarg, 0.0, 1.0));
            break;

        case 'f':
            printf("%.6g\n", flagtof(ch, optarg, NAN, NAN));
            break;

        case 'l':
            printf("%lld\n", flagtoll(ch, optarg, -255LL, 255LL));
            break;

        case 'u':
            printf("%lu\n", flagtoul(ch, optarg, 0UL, (unsigned long) INT_MAX));
            break;

        case 'D':
            numdoubles =
                flagtolods(ch, optarg, 0.0, 1.0,
                           &thedoubles, NULL, 0UL, MAX_DOUBLES);

            printf("doubles %lu/%lu - ", numdoubles, MAX_DOUBLES);
            for (size_t i = 0; i < numdoubles; i++) {
                printf("%.2f ", thedoubles[i]);
            }
            putchar('\n');
            break;

        case 'L':
            flagtololls(ch, optarg, LLONG_MIN,
                        LLONG_MAX, &thelolls, &numlls, 1UL, MAX_LOLLS);

            printf("long longs %lu/%lu - ", numlls, MAX_LOLLS);
            for (size_t i = 0; i < numlls; i++) {
                printf("%lld ", thelolls[i]);
            }
            putchar('\n');
            break;

        case 'U':
            numuls =
                flagtolouls(ch, optarg, 0UL, ULONG_MAX, &theulols, NULL,
                            1UL, MAX_ULOLS);

            printf("ulongs %lu/%lu - ", numuls, MAX_ULOLS);
            for (size_t i = 0; i < numuls; i++) {
                printf("%lu ", theulols[i]);
            }
            putchar('\n');
            break;

        case 'h':
        case '?':
        default:
            emit_help();
            /* NOTREACHED */
        }
    }
    argc -= optind;
    argv += optind;

    free(thedoubles);
    free(thelolls);
    free(theulols);

    exit(EXIT_SUCCESS);
}

void emit_help(void)
{
    fprintf(stderr,
            "Usage: goptfoo [-d double] [-f float] [-l llong] [-u ulong]"
            " [-D doubles] [-L llongs] [-U ulongs]\n");
    exit(EX_USAGE);
}
