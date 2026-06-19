#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

/**
 * `quick_type` should not be 4=sp
 */
uint64_t
get_L (uint64_t quick_ktype)
{
    switch (quick_ktype) {
        case 1:
            return 0;
        case 3:
            return 1;
        case 6:
            return 2;
        case 10:
            return 3;
        default:
            return 0;
    }
}

#endif
