#ifndef T3_LOGIC_CORE_H
#define T3_LOGIC_CORE_H

#include <stddef.h>
#include <stdint.h>

#include "t3_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#if defined(T3_LOGIC_CORE_BUILD_DLL)
#define T3_API __declspec(dllexport)
#elif defined(T3_LOGIC_CORE_USE_DLL)
#define T3_API __declspec(dllimport)
#else
#define T3_API
#endif
#else
#define T3_API
#endif

enum {
    T3_OK = 0,
    T3_ERR_INVALID_ARGUMENT = -1,
    T3_ERR_BOUNDS = -2,
    T3_ERR_BAD_OPCODE = -3
};

enum {
    T3_LOG_ERROR = 1,
    T3_LOG_INFO = 2
};

typedef struct t3_scan_buffers {
    const uint8_t *bytecode;
    size_t bytecode_len;

    const int32_t *inputs;
    size_t input_count;

    int32_t *outputs;
    size_t output_count;

    int32_t *registers;
    size_t register_count;
} t3_scan_buffers_t;

T3_API int32_t T3_Execute_Scan(const t3_platform_t *platform, t3_scan_buffers_t *buffers);

#ifdef __cplusplus
}
#endif

#endif /* T3_LOGIC_CORE_H */
