#include "t3_platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    uint32_t a = t3_monotonic_ms();
    struct timespec ts = {0, 10 * 1000 * 1000};

    if (nanosleep(&ts, NULL) != 0) {
        perror("nanosleep");
        return 1;
    }

    uint32_t b = t3_monotonic_ms();
    if (b < a) {
        fprintf(stderr, "t3_monotonic_ms went backwards (%u -> %u)\n", a, b);
        return 1;
    }

    puts("smoke ok");
    return 0;
}
