#ifndef T3_PLATFORM_H
#define T3_PLATFORM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Milliseconds from a monotonic clock (origin arbitrary).
 * Used for scan pacing and timing in the logic core.
 */
uint32_t t3_monotonic_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* T3_PLATFORM_H */
