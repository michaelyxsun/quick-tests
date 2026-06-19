#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

// #include <cuest.h>
#include "/Users/msun/rehs2026/cuest/libcuest-linux-sbsa-0.1.1.1_cuda13-archive/include/cuest.h"

void
checkCuestErrors (cuestStatus_t stat)
{
    // generated with ChatGPT
    switch (stat) {
        case CUEST_STATUS_SUCCESS:
            break;

        case CUEST_STATUS_EXCEPTION:
            fprintf (stderr, "cuEST error: an exception was thrown during the "
                             "operation.\n");
            break;

        case CUEST_STATUS_NULL_POINTER:
            fprintf (stderr, "cuEST error: one or more required input "
                             "pointers were NULL.\n");
            break;

        case CUEST_STATUS_INVALID_ARGUMENT:
            fprintf (stderr,
                     "cuEST error: one or more arguments were invalid.\n");
            break;

        case CUEST_STATUS_INVALID_SIZE:
            fprintf (stderr, "cuEST error: input buffer/argument size did not "
                             "match expected size.\n");
            break;

        case CUEST_STATUS_INVALID_TYPE:
            fprintf (
                stderr,
                "cuEST error: the specified cuEST type does not exist.\n");
            break;

        case CUEST_STATUS_INVALID_PARAMETER:
            fprintf (
                stderr,
                "cuEST error: the parameter being queried/set is invalid.\n");
            break;

        case CUEST_STATUS_INVALID_ATTRIBUTE:
            fprintf (stderr,
                     "cuEST error: the attribute being queried is invalid.\n");
            break;

        case CUEST_STATUS_INVALID_HANDLE:
            fprintf (stderr, "cuEST error: the cuEST handle passed to the "
                             "function is invalid.\n");
            break;

        case CUEST_STATUS_UNKNOWN_ERROR:
            fprintf (stderr, "cuEST error: unspecified error (unknown or "
                             "unexpected condition).\n");
            break;

        case CUEST_STATUS_UNSUPPORTED_ARGUMENT:
            fprintf (stderr, "cuEST error: one or more arguments are not "
                             "supported by cuEST.\n");
            break;

        case CUEST_STATUS_UNSUPPORTED_ARCHITECTURE:
            fprintf (stderr, "cuEST error: the device architecture is not "
                             "supported by cuEST.\n");
            break;

        default:
            fprintf (stderr, "cuEST error: unrecognized status code (%d).\n",
                     (int)stat);
            break;
    }
}

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
