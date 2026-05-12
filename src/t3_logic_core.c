#include "t3_logic_core.h"

#define T3_OP_END 0x00u
#define T3_OP_LOAD_INPUT 0x10u
#define T3_OP_LOAD_REG 0x11u
#define T3_OP_STORE_REG 0x12u
#define T3_OP_STORE_OUTPUT 0x13u
#define T3_OP_LOAD_IMM32 0x20u
#define T3_OP_ADD_IMM32 0x21u

static void t3_log(const t3_platform_t *platform, int level, const char *message) {
    if (platform != NULL && platform->log != NULL) {
        platform->log(platform->user_ctx, level, message);
    }
}

static int32_t read_u16(const uint8_t *bytecode, size_t len, size_t *pc, uint16_t *value) {
    if (*pc + 2u > len) {
        return T3_ERR_BOUNDS;
    }

    *value = (uint16_t)bytecode[*pc] | (uint16_t)((uint16_t)bytecode[*pc + 1u] << 8u);
    *pc += 2u;
    return T3_OK;
}

static int32_t read_i32(const uint8_t *bytecode, size_t len, size_t *pc, int32_t *value) {
    if (*pc + 4u > len) {
        return T3_ERR_BOUNDS;
    }

    *value = (int32_t)((uint32_t)bytecode[*pc] |
                       ((uint32_t)bytecode[*pc + 1u] << 8u) |
                       ((uint32_t)bytecode[*pc + 2u] << 16u) |
                       ((uint32_t)bytecode[*pc + 3u] << 24u));
    *pc += 4u;
    return T3_OK;
}

int32_t T3_Execute_Scan(const t3_platform_t *platform, t3_scan_buffers_t *buffers) {
    size_t pc = 0u;
    int32_t acc = 0;

    if (buffers == NULL || buffers->bytecode == NULL || (buffers->register_count > 0u && buffers->registers == NULL) ||
        (buffers->input_count > 0u && buffers->inputs == NULL) ||
        (buffers->output_count > 0u && buffers->outputs == NULL)) {
        t3_log(platform, T3_LOG_ERROR, "invalid scan buffers");
        return T3_ERR_INVALID_ARGUMENT;
    }

    while (pc < buffers->bytecode_len) {
        uint8_t opcode = buffers->bytecode[pc++];
        uint16_t index = 0u;
        int32_t imm = 0;

        switch (opcode) {
        case T3_OP_END:
            return T3_OK;

        case T3_OP_LOAD_INPUT:
            if (read_u16(buffers->bytecode, buffers->bytecode_len, &pc, &index) != T3_OK || index >= buffers->input_count) {
                t3_log(platform, T3_LOG_ERROR, "input read out of bounds");
                return T3_ERR_BOUNDS;
            }
            acc = buffers->inputs[index];
            break;

        case T3_OP_LOAD_REG:
            if (read_u16(buffers->bytecode, buffers->bytecode_len, &pc, &index) != T3_OK || index >= buffers->register_count) {
                t3_log(platform, T3_LOG_ERROR, "register read out of bounds");
                return T3_ERR_BOUNDS;
            }
            acc = buffers->registers[index];
            break;

        case T3_OP_STORE_REG:
            if (read_u16(buffers->bytecode, buffers->bytecode_len, &pc, &index) != T3_OK || index >= buffers->register_count) {
                t3_log(platform, T3_LOG_ERROR, "register write out of bounds");
                return T3_ERR_BOUNDS;
            }
            buffers->registers[index] = acc;
            break;

        case T3_OP_STORE_OUTPUT:
            if (read_u16(buffers->bytecode, buffers->bytecode_len, &pc, &index) != T3_OK || index >= buffers->output_count) {
                t3_log(platform, T3_LOG_ERROR, "output write out of bounds");
                return T3_ERR_BOUNDS;
            }
            buffers->outputs[index] = acc;
            break;

        case T3_OP_LOAD_IMM32:
            if (read_i32(buffers->bytecode, buffers->bytecode_len, &pc, &imm) != T3_OK) {
                t3_log(platform, T3_LOG_ERROR, "immediate read out of bounds");
                return T3_ERR_BOUNDS;
            }
            acc = imm;
            break;

        case T3_OP_ADD_IMM32:
            if (read_i32(buffers->bytecode, buffers->bytecode_len, &pc, &imm) != T3_OK) {
                t3_log(platform, T3_LOG_ERROR, "immediate read out of bounds");
                return T3_ERR_BOUNDS;
            }
            acc += imm;
            break;

        default:
            t3_log(platform, T3_LOG_ERROR, "bad opcode");
            return T3_ERR_BAD_OPCODE;
        }
    }

    t3_log(platform, T3_LOG_ERROR, "program terminated without END opcode");
    return T3_ERR_BOUNDS;
}
