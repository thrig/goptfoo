/* goptfoo(3) - utility functions for command-line option parsing */

#ifndef _H_LIBGOPTFOO_H_
#define _H_LIBGOPTFOO_H_

#ifdef __linux__
#define _BSD_SOURCE
#define _GNU_SOURCE
#endif

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

// non-option named arguments (from argv[1] or similar)
double argtod(const char *argname, const char *arg, const double min,
                     const double max);
long long argtoll(const char *argname, const char *arg, const long long min,
                  const long long max);
unsigned long argtoul(const char *argname, const char *arg,
                      const unsigned long min, const unsigned long max);

// single value calls for -f 0.21 or -k 5
double flagtod(const int flag, const char *flagarg,
               const double min, const double max);
long long flagtoll(const int flag, const char *flagarg,
                   const long long min, const long long max);
unsigned long flagtoul(const int flag, const char *flagarg,
                       const unsigned long min, const unsigned long max);

/* flagtof was removed in goptfoo version 5.0. instead use flagtod and
 * cast if you do need a float. also note that 32-bit floats are
 * somewhat inaccurate and start confusing integers as low as 16777216 */

// list of values -d '5 4 9 3' to be populated into the supplied **items
size_t flagtolods(const int flag, const char *flagarg,
                  const double min, const double max,
                  double **items, size_t * numitems,
                  const size_t list_min, size_t list_max);
size_t flagtololls(const int flag, const char *flagarg,
                   const long long min, const long long max,
                   long long **items, size_t * numitems,
                   const size_t list_min, size_t list_max);
size_t flagtolouls(const int flag, const char *flagarg,
                   const unsigned long min, const unsigned long max,
                   unsigned long **items, size_t * numitems,
                   const size_t list_min, size_t list_max);

#endif
