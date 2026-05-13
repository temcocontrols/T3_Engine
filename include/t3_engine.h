#ifndef T3_ENGINE_H
#define T3_ENGINE_H

/*
 * T3Engine — host-facing C API (hooks the portable core needs from the OS or BSP).
 *
 * This is the T3Engine product boundary at the C level: the logic core includes
 * this header; each target (Windows DLL, ESP-IDF firmware, POSIX smoke host)
 * supplies implementations. Older docs may say "platform"; the name of the
 * library/product is still T3Engine.
 */

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

#endif /* T3_ENGINE_H */
