#ifndef T3_PLATFORM_H
#define T3_PLATFORM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*t3_millis_fn)(void *user_ctx);
typedef void (*t3_log_fn)(void *user_ctx, int level, const char *message);

typedef struct t3_platform {
    t3_millis_fn millis;
    t3_log_fn log;
    void *user_ctx;
} t3_platform_t;

#ifdef __cplusplus
}
#endif

#endif /* T3_PLATFORM_H */
