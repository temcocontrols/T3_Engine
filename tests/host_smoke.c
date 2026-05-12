#include "t3_logic_core.h"

#include <assert.h>
#include <stdint.h>

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

    return 0;
}
