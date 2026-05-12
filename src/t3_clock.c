#include "t3_platform.h"

#if defined(_WIN32) && defined(_MSC_VER)

#include <windows.h>

uint32_t t3_monotonic_ms(void)
{
    return (uint32_t)GetTickCount64();
}

#else

#include <time.h>

uint32_t t3_monotonic_ms(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return (uint32_t)((ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL));
}

#endif
