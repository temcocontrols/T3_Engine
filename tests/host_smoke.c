#include "t3_logic_core.h"

#include <assert.h>
#include <stdint.h>

typedef struct test_log_state {
    int calls;
    int last_level;
} test_log_state_t;

static void test_log_callback(void *user_ctx, int level, const char *message) {
    test_log_state_t *state = (test_log_state_t *)user_ctx;
    (void)message;
    state->calls++;
    state->last_level = level;
}

int main(void) {
    const uint8_t bytecode[] = {
        0x10, 0x00, 0x00,
        0x21, 0x05, 0x00, 0x00, 0x00,
        0x12, 0x00, 0x00,
        0x13, 0x01, 0x00,
        0x00,
    };

    int32_t inputs[] = {7};
    int32_t outputs[] = {0, 0};
    int32_t registers[] = {0};

    t3_scan_buffers_t buffers = {
        .bytecode = bytecode,
        .bytecode_len = sizeof(bytecode),
        .inputs = inputs,
        .input_count = sizeof(inputs) / sizeof(inputs[0]),
        .outputs = outputs,
        .output_count = sizeof(outputs) / sizeof(outputs[0]),
        .registers = registers,
        .register_count = sizeof(registers) / sizeof(registers[0]),
    };

    assert(T3_Execute_Scan(NULL, &buffers) == T3_OK);
    assert(registers[0] == 12);
    assert(outputs[1] == 12);

    {
        const uint8_t bad_bytecode[] = {0xFF};
        test_log_state_t log_state = {0};
        t3_platform_t platform = {
            .millis = NULL,
            .log = test_log_callback,
            .user_ctx = &log_state,
        };

        buffers.bytecode = bad_bytecode;
        buffers.bytecode_len = sizeof(bad_bytecode);
        assert(T3_Execute_Scan(&platform, &buffers) == T3_ERR_BAD_OPCODE);
        assert(log_state.calls == 1);
        assert(log_state.last_level == T3_LOG_ERROR);
    }

    return 0;
}
