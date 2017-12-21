/* example script for goptfoo(3) that shows how its various functions
 * can be integrated with getopt(3). compile with something like
 *
 *   cc -c `pkg-config --cflags goptfoo` -g -std=c99 goptfoo.c -o gof.o
 *   ld `pkg-config --libs goptfoo` -lc gof.o -o getoptfoo
 */

#ifdef __linux__
#include <getopt.h>
#endif

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

// https://github.com/thrig/goptfoo
#include <goptfoo.h>

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

    /* pre-population is supported, for beter or worse */
    if ((thelolls = malloc((size_t) 7 * sizeof(long long))) == NULL)
        err(EX_OSERR, "could not allocate list of lolls");
    for (size_t i = 0; i < 7; i++) {
        thelolls[i] = 100 + i;
    }
    /* numdoubles is not passed in unlike these two, so need not be
     * set. however, it is probably safer to do so, in the event
     * something changes */
    numlls = 7;
    numuls = 0;

    while ((ch = getopt(argc, argv, "h?d:f:l:u:D:L:U:")) != -1) {
        switch (ch) {
        case 'd':
            printf("%g\n", flagtod(ch, optarg, 0.0, 1.0));
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
                printf("%g ", thedoubles[i]);
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

    if (*argv != NULL) {
        printf("%g\n", argtod("dvalue", *argv, 0.0, 1.0));
        argv++;
    }
    if (*argv != NULL) {
        printf("%lld\n", argtoll("llvalue", *argv, -255LL, 255LL));
        argv++;
    }
    if (*argv != NULL) {
        printf("%lu\n",
               argtoul("ulvalue", *argv, 0UL, (unsigned long) INT_MAX));
    }

    //free(thedoubles);
    //free(thelolls);
    //free(theulols);
    exit(EXIT_SUCCESS);
}

void emit_help(void)
{
    fprintf(stderr,
            "Usage: goptfoo [-d double]  [-l llong]  [-u ulong]\n"
            "               [-D doubles] [-L llongs] [-U ulongs]\n"
            "               double llong ulong\n");
    exit(EX_USAGE);
}
