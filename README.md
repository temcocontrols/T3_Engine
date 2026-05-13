# T3Engine

Hardware-agnostic **T3 program execution core**: interpreter (planned) plus **host hooks** in `include/t3_engine.h` (time first; I/O later). Same codebase is intended to compile for **ESP-IDF** on controllers and as a **Windows DLL** for tools such as T3000.

## Quick orientation (for reviewers and integrators)

| Question | Answer |
|----------|--------|
| Is this the whole T3000 app? | **No.** T3000 is a Windows MFC application in its own repo. This tree is only the **shared logic core** (and minimal host tests). |
| Why does GitHub Actions use **Linux**? | To get a **zero-setup `gcc`** on every push: strict compile + run a tiny test. It validates **portable C**, not “T3000 on Linux.” See below. |
| How do I smoke-test on **Windows**? | Run `scripts/build-smoke.ps1` (uses WinLibs MinGW `gcc` if MSVC is not used for this step). |
| Where is the **MSVC / vcxproj / DLL** wiring? | In **T3000** (or firmware) repos when they submodule or vendor this code—not in this smoke job. |

### Why CI uses Linux and `gcc`

T3000 (the building-automation client) is a **Windows** application and may consume this core as native code from its own solution. **This repo is still multi-target**: the same headers and logic are meant for controllers (ESP-IDF) and for Windows DLLs, with platform-specific glue where needed.

GitHub Actions runs the smoke job on **Ubuntu with `gcc`** because that image already includes a C toolchain with no Visual Studio install or licensing setup. The job checks that the **shared C** compiles with strict warnings and that the POSIX host clock implementation passes the smoke test. It does **not** imply that T3000 ships or runs on Linux.

For a **Windows** machine, use `scripts/build-smoke.ps1`, which builds the same sources with WinLibs MinGW `gcc` when MSVC is not involved. **MSVC builds, project files, and shipping the DLL inside T3000** live in the T3000 (or firmware) repository; they are separate from this minimal POSIX smoke path.

### What the smoke test checks

The smoke binary links `tests/smoke_main.c` with `src/t3_clock.c`. It calls `t3_monotonic_ms()`, sleeps briefly, then calls it again and **fails** if time appears to go backwards. That guards the timing hook used for scan pacing and similar logic in the real core.

## Layout (this repo only)

| Path | Purpose |
|------|--------|
| `include/t3_engine.h` | T3Engine host hooks the core calls (time first; I/O later). |
| `include/temco_product_desc.h` | Shared **product descriptor** struct and helpers (**VID** / **PID** and layout constants); used by ESP32 `pro_info` and PC tools that read the same blob. |
| `src/t3_clock.c` | Default **host/POSIX** `t3_monotonic_ms()` for CI and smoke tests. |
| `tests/smoke_main.c` | Minimal program to verify the clock hook. |
| `.github/workflows/ci.yml` | Linux `gcc` smoke build; job name **`build`** for branch rulesets. |
| `scripts/build-smoke.ps1` | **Windows:** finds WinGet WinLibs `gcc` if it is not on `PATH`, then builds and runs the smoke test. |
| `T3_Logic_Core.code-workspace` | **Cursor / VS Code:** open this file for a single-root workspace with include paths for IntelliSense. **Visual Studio** (devenv) does not use `.code-workspace` files; open the **T3Engine** folder (**File → Open → Folder**) or work from the **T3000** solution that submodules this repo. |

Firmware and T3000 stay in their own repos; they will add this tree as a **submodule** when ready.

## Local smoke build (Windows)

1. Install GCC (WinLibs): `winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT` (then restart the terminal or PC if `gcc` is not found).
2. From repo root: `powershell -ExecutionPolicy Bypass -File scripts/build-smoke.ps1`

Expected: the script prints `Using: …\gcc.exe`, then `smoke ok`.

## Local smoke build (Linux / macOS)

Same command as CI (from repo root). Requires `gcc` (or `cc`) with C11 and POSIX clocks.

```bash
gcc -std=c11 -Wall -Wextra -Werror -Iinclude -D_DEFAULT_SOURCE \
  tests/smoke_main.c src/t3_clock.c -o smoke
./smoke
```

Expected terminal output: `smoke ok`. Remove `./smoke` when done if you like; the binary is gitignored at repo root.

## Roadmap and PR notes

Integration goals, BACnet reuse strategy, and a **suggested GitHub PR description** (including the `t3_engine.h` rename) live in **[`docs/NEXT-STEPS.md`](docs/NEXT-STEPS.md)**.
