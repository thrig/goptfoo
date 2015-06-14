#ifndef _H_LIBGOPTFOO_H_
#define _H_LIBGOPTFOO_H_

#ifdef __linux__
#define _BSD_SOURCE
#define _GNU_SOURCE
#include <getopt.h>
#endif

#include <err.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

double flagtod(const int flag, const char *flagarg,
               const double min, const double max);
float flagtof(const int flag, const char *flagarg,
              const float min, const float max);
long long flagtoll(const int flag, const char *flagarg,
                   const long long min, const long long max);
unsigned long flagtoul(const int flag, const char *flagarg,
                       const unsigned long min, const unsigned long max);

#endif
