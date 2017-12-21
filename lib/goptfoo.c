/* goptfoo(3) - utility functions for command-line option parsing */

/* NOTE "-3" strtoul() will helpfully convert to 18446744073709551613
 * (without error) thus either setting a surprisingly large value or
 * failing with a confusing error message that -3 is less than the max.
 * therefore, negative values are not allowed to the unsigned functions
 * of this library */

#include "goptfoo.h"

/* argto* ************************************************************* */

double argtod(const char *argname, const char *arg, const double min,
              const double max)
{
    char *ep;
    double val;

    if (!arg || *arg == '\0')
        errx(EX_DATAERR, "%s value not set", argname);

    errno = 0;
    val = strtod(arg, &ep);
    if (arg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtod failed on %s value '%s'", argname, arg);
    if (errno == ERANGE)
        errx(EX_DATAERR, "value for %s is not a double", argname);
    if (isfinite(min) && val < min)
        errx(EX_DATAERR, "value for %s is below min %g", argname, min);
    if (isfinite(max) && val > max)
        errx(EX_DATAERR, "value for %s is above max %g", argname, max);

    return val;
}

long long argtoll(const char *argname, const char *arg, const long long min,
                  const long long max)
{
    char *ep;
    long long val;

    if (!arg || *arg == '\0')
        errx(EX_DATAERR, "%s value not set", argname);

    errno = 0;
    val = strtoll(arg, &ep, 0);
    if (arg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtoll failed on %s value '%s'", argname, arg);
    if (errno == ERANGE && (val == LLONG_MIN || val == LLONG_MAX))
        errx(EX_DATAERR, "%s value is not a long long", argname);
    if (min != LLONG_MIN && val < min)
        errx(EX_DATAERR, "%s value is below min %lld", argname, min);
    if (max != LLONG_MAX && val > max)
        errx(EX_DATAERR, "%s value is above max %lld", argname, max);

    return val;
}

unsigned long argtoul(const char *argname, const char *arg,
                      const unsigned long min, const unsigned long max)
{
    char *ep;
    unsigned long val;

    if (!arg || *arg == '\0')
        errx(EX_DATAERR, "%s value not set", argname);

    while (isspace(*arg))
        arg++;
    if (*arg != '+' && !isdigit(*arg))
        errx(EX_DATAERR, "%s value must be positive", argname);

    errno = 0;
    val = strtoul(arg, &ep, 0);
    if (arg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtoul failed on %s value '%s'", argname, arg);
    if (errno == ERANGE && val == ULONG_MAX)
        errx(EX_DATAERR, "%s value is not an unsigned long", argname);
    if (min != 0 && val < min)
        errx(EX_DATAERR, "%s value is below min %lu", argname, min);
    if (max != ULONG_MAX && val > max)
        errx(EX_DATAERR, "%s value is above max %lu", argname, max);

    return val;
}

/* flagto* ************************************************************ */

double flagtod(const int flag, const char *flagarg,
               const double min, const double max)
{
    char *ep;
    double val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    errno = 0;
    val = strtod(flagarg, &ep);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtod failed on -%c value '%s'", flag, flagarg);
    if (errno == ERANGE)
        errx(EX_DATAERR, "value for -%c is not a double", flag);
    if (isfinite(min) && val < min)
        errx(EX_DATAERR, "value for -%c is below min %g", flag, min);
    if (isfinite(max) && val > max)
        errx(EX_DATAERR, "value for -%c is above max %g", flag, max);

    return val;
}

long long flagtoll(const int flag, const char *flagarg,
                   const long long min, const long long max)
{
    char *ep;
    long long val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    errno = 0;
    val = strtoll(flagarg, &ep, 0);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtoll failed on -%c value '%s'", flag, flagarg);
    if (errno == ERANGE && (val == LLONG_MIN || val == LLONG_MAX))
        errx(EX_DATAERR, "value for -%c is not a long long", flag);
    if (min != LLONG_MIN && val < min)
        errx(EX_DATAERR, "value for -%c is below min %lld", flag, min);
    if (max != LLONG_MAX && val > max)
        errx(EX_DATAERR, "value for -%c is above max %lld", flag, max);

    return val;
}

unsigned long flagtoul(const int flag, const char *flagarg,
                       const unsigned long min, const unsigned long max)
{
    char *ep;
    unsigned long val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    while (isspace(*flagarg))
        flagarg++;
    if (*flagarg != '+' && !isdigit(*flagarg))
        errx(EX_DATAERR, "flag -%c value must be positive", flag);

    errno = 0;
    val = strtoul(flagarg, &ep, 0);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "strtoul failed on -%c value '%s'", flag, flagarg);
    if (errno == ERANGE && val == ULONG_MAX)
        errx(EX_DATAERR, "value for -%c is not an unsigned long", flag);
    if (min != 0 && val < min)
        errx(EX_DATAERR, "value for -%c is below min %lu", flag, min);
    if (max != ULONG_MAX && val > max)
        errx(EX_DATAERR, "value for -%c is above max %lu", flag, max);

    return val;
}

/* flagtolo*s ********************************************************* */

/* there are three variables related to the number of items in these
 * flagtolo* functions; numi is sent back to the user, and should be the
 * number of items (and also *numitems unless that was NULL); list_max
 * sets a user-specified (or at most the data type allows to avoid
 * size_t overflows) high water mark, while the alloitems variable is
 * for how many items have been allocated, which will usually be above
 * the number of items and hopefully not beyond list_max */

size_t flagtolods(const int flag, const char *flagarg,
                  const double min, const double max,
                  double **items, size_t * numitems,
                  const size_t list_min, size_t list_max)
{
    char *ep;
    double value;
    size_t alloitems = 8;       /* arbitrary starting point */
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    if (list_max > SIZE_MAX / sizeof(double))
        errx(EX_DATAERR, "list_max %lu too large for double", list_max);
    else if (list_max == 0)
        list_max = SIZE_MAX / sizeof(double);

    if (numitems) {
        if (*numitems > list_max) {
            errx(EX_DATAERR, "too many items in list already");
        } else if (*numitems > 0) {
            alloitems = *numitems << 1;
        }
        numi = *numitems;
    }
    /* resize items to allocate down, or up if the provided list_max is
     * "close" to the starting number of items to allocate */
    if (alloitems > list_max || (alloitems << 2) > list_max)
        alloitems = list_max;

    /* really want reallocarray(3) but that's not portable enough yet */
    if ((*items = realloc(*items, alloitems * sizeof(double))) == NULL)
        err(EX_OSERR, "could not realloc() %ld doubles for -%c",
            alloitems, flag);

    while (*flagarg != '\0') {
        if (isspace(*flagarg)) {
            flagarg++;
            continue;
        }
        /* strtod(3) accepts ".95" as well as "0.95" so support that */
        if (*flagarg != '+' && *flagarg != '.' && *flagarg != '-'
            && !isdigit(*flagarg))
            errx(EX_DATAERR, "non-digit non-space character in -%c", flag);

        errno = 0;
        value = strtod(flagarg, &ep);

        /* no movement on pointer. can be caused by " + " or such in the
         * input. cannot do usual '\0' check as more numbers may follow,
         * though do need some means to detect such cases */
        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse double in -%c", flag);

        if (errno == ERANGE)
            errx(EX_DATAERR, "value for -%c is not a double", flag);
        if (isfinite(min) && value < min)
            errx(EX_DATAERR, "value for -%c is below min %g", flag, min);
        if (isfinite(max) && value > max)
            errx(EX_DATAERR, "value for -%c is above max %g", flag, max);

        (*items)[numi++] = value;

        if (numi >= alloitems) {
            alloitems <<= 1;
            if (alloitems > list_max)
                alloitems = list_max;

            if ((*items =
                 realloc(*items, (size_t) alloitems * sizeof(double))) == NULL)
                err(EX_OSERR, "could not realloc() %ld doubles for -%c",
                    alloitems, flag);
        }
        /* probably only relevant if numi bumps up against list_max or a
         * cosmic ray upsized *numitems or who knows */
        if (numi > alloitems)
            errx(EX_OSERR, "too many values specified for -%c, need only %ld",
                 flag, list_max);

        flagarg = ep;
    }

    if (list_min > 0 && numi < list_min)
        errx(EX_DATAERR, "too few values specified for -%c, need %ld", flag,
             list_min);

    if (numitems)
        *numitems = numi;

    return numi;
}

size_t flagtololls(const int flag, const char *flagarg,
                   const long long min, const long long max,
                   long long **items, size_t * numitems,
                   const size_t list_min, size_t list_max)
{
    char *ep;
    long long value;
    size_t alloitems = 8;       /* arbitrary starting point */
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    if (list_max > SIZE_MAX / sizeof(long long))
        errx(EX_DATAERR, "list_max %lu too large for long long", list_max);
    else if (list_max == 0)
        list_max = SIZE_MAX / sizeof(long long);

    if (numitems) {
        if (*numitems > list_max) {
            errx(EX_DATAERR, "too many items in list already");
        } else if (*numitems > 0) {
            alloitems = *numitems << 1;
        }
        numi = *numitems;
    }

    if (alloitems > list_max || (alloitems << 2) > list_max)
        alloitems = list_max;

    if ((*items = realloc(*items, alloitems * sizeof(long long))) == NULL)
        err(EX_OSERR, "could not realloc() %ld long longs for -%c", alloitems,
            flag);

    while (*flagarg != '\0') {
        if (isspace(*flagarg)) {
            flagarg++;
            continue;
        }
        if (*flagarg != '+' && *flagarg != '-' && !isdigit(*flagarg))
            errx(EX_DATAERR, "non-digit non-space character in -%c", flag);

        errno = 0;
        value = strtoll(flagarg, &ep, 0);

        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse long long in -%c", flag);

        if (errno == ERANGE && (value == LLONG_MIN || value == LLONG_MAX))
            errx(EX_DATAERR, "value for -%c is not a long long", flag);
        if (min != LLONG_MIN && value < min)
            errx(EX_DATAERR, "value for -%c is below min %lld", flag, min);
        if (max != LLONG_MAX && value > max)
            errx(EX_DATAERR, "value for -%c is above max %lld", flag, max);

        (*items)[numi++] = value;

        if (numi >= alloitems) {
            alloitems <<= 1;
            if (alloitems > list_max)
                alloitems = list_max;

            if ((*items =
                 realloc(*items,
                         (size_t) alloitems * sizeof(long long))) == NULL)
                err(EX_OSERR, "could not realloc() %ld long longs for -%c",
                    alloitems, flag);
        }
        if (numi > alloitems)
            errx(EX_OSERR, "too many values specified for -%c, need only %ld",
                 flag, list_max);

        flagarg = ep;
    }

    if (list_min > 0 && numi < list_min)
        errx(EX_DATAERR, "too few values specified for -%c, need %ld", flag,
             list_min);

    if (numitems)
        *numitems = numi;

    return numi;
}

size_t flagtolouls(const int flag, const char *flagarg,
                   const unsigned long min, const unsigned long max,
                   unsigned long **items, size_t * numitems,
                   const size_t list_min, size_t list_max)
{
    char *ep;
    unsigned long value;
    size_t alloitems = 8;       /* arbitrary starting point */
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag -%c value not set", flag);

    if (list_max > SIZE_MAX / sizeof(unsigned long))
        errx(EX_DATAERR, "list_max %lu too large for unsigned long", list_max);
    else if (list_max == 0)
        list_max = SIZE_MAX / sizeof(unsigned long);

    if (numitems) {
        if (*numitems > list_max) {
            errx(EX_DATAERR, "too many items in list already");
        } else if (*numitems > 0) {
            alloitems = *numitems << 1;
        }
        numi = *numitems;
    }

    if (alloitems > list_max || (alloitems << 2) > list_max)
        alloitems = list_max;

    if ((*items = realloc(*items, alloitems * sizeof(unsigned long))) == NULL)
        err(EX_OSERR, "could not realloc() %ld unsigned longs for -%c",
            alloitems, flag);

    while (*flagarg != '\0') {
        if (isspace(*flagarg)) {
            flagarg++;
            continue;
        }

        if (*flagarg != '+' && !isdigit(*flagarg))
            errx(EX_DATAERR,
                 "non-positive integer non-space character found in -%c", flag);

        errno = 0;
        value = strtoul(flagarg, &ep, 0);

        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse unsigned long in -%c", flag);

        if (errno == ERANGE && value == ULONG_MAX)
            errx(EX_DATAERR, "value for -%c is not an unsigned long", flag);
        if (min != 0 && value < min)
            errx(EX_DATAERR, "value for -%c is below min %lu", flag, min);
        if (max != ULONG_MAX && value > max)
            errx(EX_DATAERR, "value for -%c is above max %lu", flag, max);

        (*items)[numi++] = value;

        if (numi >= alloitems) {
            alloitems <<= 1;
            if (alloitems > list_max)
                alloitems = list_max;

            if ((*items =
                 realloc(*items,
                         (size_t) alloitems * sizeof(unsigned long))) == NULL)
                err(EX_OSERR, "could not realloc() %ld unsigned longs for -%c",
                    alloitems, flag);
        }
        if (numi > alloitems)
            errx(EX_OSERR, "too many values specified for -%c, need only %ld",
                 flag, list_max);

        flagarg = ep;
    }

    if (list_min > 0 && numi < list_min)
        errx(EX_DATAERR, "too few values specified for -%c, need %ld", flag,
             list_min);

    if (numitems)
        *numitems = numi;

    return numi;
}
