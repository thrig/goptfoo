/* goptfoo(3) - utility functions for command-line option parsing. */

#include "goptfoo.h"

double flagtod(const int flag, const char *flagarg,
               const double min, const double max)
{
    char *ep;
    double val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

    errno = 0;
    val = strtod(flagarg, &ep);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "could not parse double from -%c '%s'", flag, flagarg);
    if (errno == ERANGE)
        errx(EX_DATAERR, "value for -%c '%s' is not a double", flag, flagarg);
    if (isfinite(min) && val < min)
        errx(EX_DATAERR, "value for -%c '%s' is below min %.2f", flag, flagarg,
             min);
    if (isfinite(max) && val > max)
        errx(EX_DATAERR, "value for -%c '%s' exceeds max %.2f", flag, flagarg,
             max);
    return val;
}

float flagtof(const int flag, const char *flagarg,
              const float min, const float max)
{
    char *ep;
    float val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

    errno = 0;
    val = strtof(flagarg, &ep);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "could not parse float from -%c '%s'", flag, flagarg);
    if (errno == ERANGE)
        errx(EX_DATAERR, "value for -%c '%s' is not a float", flag, flagarg);
    if (isfinite(min) && val < min)
        errx(EX_DATAERR, "value for -%c '%s' is below min %.2f", flag, flagarg,
             min);
    if (isfinite(max) && val > max)
        errx(EX_DATAERR, "value for -%c '%s' exceeds max %.2f", flag, flagarg,
             max);
    return val;
}

long long flagtoll(const int flag, const char *flagarg,
                   const long long min, const long long max)
{
    char *ep;
    long long val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

    errno = 0;
    val = strtoll(flagarg, &ep, 10);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "could not parse long long from -%c '%s'", flag,
             flagarg);
    if (errno == ERANGE && (val == LLONG_MIN || val == LLONG_MAX))
        errx(EX_DATAERR, "value for -%c '%s' out of range", flag, flagarg);
    if (min != LLONG_MIN && val < min)
        errx(EX_DATAERR, "value for -%c '%s' is below min %lld", flag, flagarg,
             min);
    if (max != LLONG_MAX && val > max)
        errx(EX_DATAERR, "value for -%c '%s' exceeds max %lld", flag, flagarg,
             max);
    return val;
}


/* There are three variables related to the number of items in these
 * flagtolo* functions; numi is sent back to the user, and should be the
 * number of items (and also *numitems unless that was NULL); list_max
 * sets a user-specified (or at most the data type allows to avoid
 * size_t overflows) high water mark, while the alloitems variable is
 * for how many items have been allocated, which will usually be above
 * the number of items and hopefully not beyond list_max.
 */

// NOTE mostly duplicates *tolls code, below. :/
size_t flagtolods(const int flag, const char *flagarg,
                  const double min, const double max,
                  double **items, size_t * numitems,
                  const size_t list_min, size_t list_max)
{
    char *ep;
    double value;
    size_t alloitems = 8;       // arbitrary starting point
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

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
    // resize items to allocate down, or up if the provided list_max is
    // "close" to the starting number of items to allocate
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
        // strtod(3) accepts ".95" as well as "0.95" so must support that
        if (*flagarg != '+' && *flagarg != '.' && *flagarg != '-'
            && !isdigit(*flagarg))
            errx(EX_DATAERR, "non-digit non-space character in -%c", flag);

        errno = 0;
        value = strtod(flagarg, &ep);

        /* No movement on pointer. Can be caused by " + " or such in the
         * input. Cannot do usual '\0' check as more numbers may follow,
         * though do need some means to detect such cases. */
        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse double in -%c", flag);

        if (errno == ERANGE)
            errx(EX_DATAERR, "value in -%c is not a double", flag);
        if (isfinite(min) && value < min)
            errx(EX_DATAERR, "value in -%c is below min %.2f", flag, min);
        if (isfinite(max) && value > max)
            errx(EX_DATAERR, "value in -%c exceeds max %.2f", flag, max);

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
        // probably only relevant if numi bumps up against list_max
        // or a cosmic ray upsized *numitems or who knows
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

// NOTE this is largely duplicated by the *louls code, below :/
size_t flagtololls(const int flag, const char *flagarg,
                   const long long min, const long long max,
                   long long **items, size_t * numitems,
                   const size_t list_min, size_t list_max)
{
    char *ep;
    long long value;
    size_t alloitems = 8;       // arbitrary starting point
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

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

    /* really want reallocarray(3) but that's not portable enough yet */
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
        value = strtoll(flagarg, &ep, 10);

        /* No movement on pointer. Can be caused by " + " or such in the
         * input. Cannot do usual '\0' check as more numbers may follow,
         * though do need some means to detect such cases. */
        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse long long in -%c", flag);

        if (errno == ERANGE && (value == LLONG_MIN || value == LLONG_MAX))
            errx(EX_DATAERR, "value in -%c out of range", flag);
        if (min != LLONG_MIN && value < min)
            errx(EX_DATAERR, "value in -%c is below min %lld", flag, min);
        if (max != LLONG_MAX && value > max)
            errx(EX_DATAERR, "value in -%c exceeds max %lld", flag, max);

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
        // probably only relevant if numi bumps up against list_max
        // or a cosmic ray upsized *numitems or who knows
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

// NOTE this largely duplicates the *lolls code, above :/
size_t flagtolouls(const int flag, const char *flagarg,
                   const unsigned long min, const unsigned long max,
                   unsigned long **items, size_t * numitems,
                   const size_t list_min, size_t list_max)
{
    char *ep;
    unsigned long value;
    size_t alloitems = 8;       // arbitrary starting point
    size_t numi = 0;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

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

    /* really want reallocarray(3) but that's not portable enough yet */
    if ((*items = realloc(*items, alloitems * sizeof(unsigned long))) == NULL)
        err(EX_OSERR, "could not realloc() %ld unsigned longs for -%c",
            alloitems, flag);

    while (*flagarg != '\0') {
        if (isspace(*flagarg)) {
            flagarg++;
            continue;
        }
        // NOTE negatives are not allowed, see comment in flagtoul()
        if (*flagarg != '+' && !isdigit(*flagarg))
            errx(EX_DATAERR,
                 "non-positive integer non-space character found in -%c", flag);

        errno = 0;
        value = strtoul(flagarg, &ep, 10);

        /* No movement on pointer. Can be caused by " + " or such in the
         * input. Cannot do usual '\0' check as more numbers may follow,
         * though do need some means to detect such cases. */
        if (flagarg == ep)
            errx(EX_DATAERR, "could not parse unsigned long in -%c", flag);

        if (errno == ERANGE && value == ULONG_MAX)
            errx(EX_DATAERR, "value in -%c out of range", flag);
        if (min != 0 && value < min)
            errx(EX_DATAERR, "value in -%c is below min %lu", flag, min);
        if (max != ULONG_MAX && value > max)
            errx(EX_DATAERR, "value in -%c exceeds max %lu", flag, max);

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
        // probably only relevant if numi bumps up against list_max
        // or a cosmic ray upsized *numitems or who knows
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


unsigned long flagtoul(const int flag, const char *flagarg,
                       const unsigned long min, const unsigned long max)
{
    char *ep;
    unsigned long val;

    if (!flagarg || *flagarg == '\0')
        errx(EX_DATAERR, "flag argument not set for -%c", flag);

    /* NOTE can pass "-3" as a value, which strtoul() helpfully converts
     * to 18446744073709551613 (without error) thus either setting a
     * surprisingly large value or failing with a confusing error
     * message that -3 is less than the max. Therefore, I do not allow
     * negative values to the *ul* functions. */
    while (isspace(*flagarg)) {
        flagarg++;
    }
    if (*flagarg != '+' && !isdigit(*flagarg))
        errx(EX_DATAERR, "value must begin with + or digit in -%c '%s'", flag,
             flagarg);

    errno = 0;
    val = strtoul(flagarg, &ep, 10);
    if (flagarg[0] == '\0' || *ep != '\0')
        errx(EX_DATAERR, "could not parse unsigned long from -%c '%s'", flag,
             flagarg);
    if (errno == ERANGE && val == ULONG_MAX)
        errx(EX_DATAERR, "value for -%c '%s' exceeds ULONG_MAX %lu", flag,
             flagarg, ULONG_MAX);
    if (min != 0 && val < min)
        errx(EX_DATAERR, "value for -%c '%s' is below min %lu", flag, flagarg,
             min);
    if (max != ULONG_MAX && val > max)
        errx(EX_DATAERR, "value for -%c '%s' exceeds max %lu", flag, flagarg,
             max);
    return val;
}
