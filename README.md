# T3_Logic_Core

Hardware-agnostic T3 program execution core: interpreter + platform hooks for ESP-IDF and Windows (DLL).

## Repository layout

- `include/` public C headers (`t3_platform.h`, `t3_logic_core.h`)
- `src/` core implementation (no ESP-IDF or RTOS includes)
- `tests/` host smoke test with synthetic bytecode

## Platform abstraction

The core takes an optional `t3_platform_t` with function pointers:

- `millis` for monotonic time (optional)
- `log` for logging (optional)
- `user_ctx` for consumer-owned context

The core itself stays hardware-agnostic and only uses caller-provided buffers and callbacks.

## Stable DLL-friendly API (v1)

Exported entry point:

- `int32_t T3_Execute_Scan(const t3_platform_t* platform, t3_scan_buffers_t* buffers);`

`buffers` is caller-owned and carries bytecode plus input/output/register memory. No heap allocation is required in the core.

### ABI / CRT rules

- Caller owns all memory (`bytecode`, `inputs`, `outputs`, `registers`).
- The core does not free caller memory and does not expect ownership transfer.
- For Windows DLL builds, use consistent CRT settings with T3000 (`/MD` for release, `/MDd` for debug).
- Do not propagate C++ exceptions across the DLL boundary; this API is plain C.

## Firmware integration guidance

Add this repository as a submodule in firmware and compile the same C sources through ESP-IDF CMake. Implement a thin firmware adapter that fills `t3_platform_t` and `t3_scan_buffers_t` from ESP-IDF data and calls `T3_Execute_Scan`.

## T3000 integration guidance (v1)

Use a non-blocking 1-second background timer/thread to:

1. Sync cached/network registers into caller-owned buffers.
2. Call `T3_Execute_Scan`.
3. Sync outputs back and detect changes.

Suggested status indicators: engine running state, last scan duration (ms), last error code.

## Build and test (host)

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Submodule pinning

Firmware and T3000 should pin the same `T3_Logic_Core` submodule commit SHA to guarantee identical interpreter behavior.
