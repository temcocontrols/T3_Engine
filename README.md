# T3_Logic_Core

Hardware-agnostic T3 program execution core: interpreter + platform hooks for ESP-IDF and Windows (DLL).

## Layout (this repo only)

| Path | Purpose |
|------|--------|
| `include/t3_platform.h` | Platform hooks the core calls (time first; I/O later). |
| `src/t3_clock.c` | Default **host/POSIX** `t3_monotonic_ms()` for CI and smoke tests. |
| `tests/smoke_main.c` | Minimal program to verify the clock hook. |
| `.github/workflows/ci.yml` | Linux `gcc` smoke build; job name **`build`** for branch rulesets. |
| `scripts/build-smoke.ps1` | **Windows:** finds WinGet WinLibs `gcc` if it is not on `PATH`, then builds and runs the smoke test. |

Firmware and T3000 stay in their own repos; they will add this tree as a **submodule** when ready.

## Local smoke build (Windows)

1. Install GCC (WinLibs): `winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT` (then restart the terminal or PC if `gcc` is not found).
2. From repo root: `powershell -ExecutionPolicy Bypass -File scripts/build-smoke.ps1`

