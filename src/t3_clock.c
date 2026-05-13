/*
 * POSIX host implementation of t3_monotonic_ms() from t3_engine.h.
 *
 * Used by this repo's Linux CI and tests/smoke_main.c (link this TU with the test).
 * Firmware (ESP-IDF, etc.) supplies its own hook when the core is linked there.
 * T3000's T3EngineDll path does not implement this hook today (shared product
 * descriptor only); nothing in T3000 replaces this file for host POSIX builds.
 */

#include "t3_engine.h"

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
