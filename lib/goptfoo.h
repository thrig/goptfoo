/* goptfoo(3) - utility functions for command-line option parsing */

#ifndef _H_LIBGOPTFOO_H_
#define _H_LIBGOPTFOO_H_

#ifdef __linux__
#define _BSD_SOURCE
#define _GNU_SOURCE
#include <getopt.h>
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

// single value calls for things like -f 0.21 -k 5
double flagtod(const int flag, const char *flagarg,
               const double min, const double max);
float flagtof(const int flag, const char *flagarg,
              const float min, const float max);
long long flagtoll(const int flag, const char *flagarg,
                   const long long min, const long long max);
unsigned long flagtoul(const int flag, const char *flagarg,
                       const unsigned long min, const unsigned long max);

// list of values e.g. -d '5 4 9 3' to be populated into the supplied **
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
