# T3Engine — PR notes and roadmap

This file supports the **first integration PRs** (documentation, `t3_engine.h` rename, CI orientation) and records **agreed direction** for later work. Update it as milestones land.

## Suggested text for the GitHub PR (T3Engine repo)

**Title:** Docs + host API header rename (`t3_platform.h` → `t3_engine.h`)

**Summary**

- Clarify in **README** why GitHub Actions uses Linux/`gcc` (portable C smoke only; T3000 remains Windows) and add a **quick-orientation** table for reviewers.
- Gitignore the POSIX smoke binary `smoke` at repo root.
- Document **Linux/macOS** one-liner smoke build (same flags as CI).
- Rename **`include/t3_platform.h`** to **`include/t3_engine.h`** so the public host hook header matches the **T3Engine** product name. The guard macro is now `T3_ENGINE_H`. Any consumer (submodule, DLL project, firmware) must update `#include` and remove references to the old filename.

**Breaking change:** `#include "t3_platform.h"` → `#include "t3_engine.h"`.

**Test plan:** Linux CI job `build`; locally run `scripts/build-smoke.ps1` (Windows) or the `gcc` command from README (POSIX host).

---

## Architecture decisions (from team discussion)

### Large engine vs. whole firmware

It is **expected** that routing, Modbus, SNMP, BACnet, and related automation logic will make this repo **large** over time. That is appropriate **if** that code is the **single source of truth** for behavior on both the **ESP32 firmware** and **T3000**, with **modular directories** (e.g. per protocol) and **host tests** on CI.

The engine should **not** try to absorb the **entire ESP-IDF board support package** (drivers, Wi‑Fi internals, flash layout). Keep **hardware and RTOS integration** in the firmware repo; keep **portable protocol and routing logic** here, calling only **host hooks** (`t3_engine.h` and successors) for time, I/O, storage, etc.

### Reusing T3000’s BACnet assets

T3000 already contains substantial **BACnet-related** work (tools, C# explorers, and C++ application code). **Leverage that** for semantics, test traffic, object rules, and institutional knowledge. **Reuse strategy depends on layer:**

- Prefer **shared C/C++** where the production path already is native and can move behind a stable API.
- Prefer **shared data and specs** (register maps, golden PDUs, generated headers) where the stack is **C# / tooling-only** and must not be forced into the ESP32 build.
- Use explorer/tool projects for **conformance and debugging**, not as a substitute for a **small, testable engine core** on the wire.

Avoid **two diverging “authoritative” BACnet implementations** without an explicit plan for which one wins long term.

---

## Near-term checklist (engineering)

1. **Consumers of the rename** — Update every submodule or copy: `#include "t3_engine.h"`, remove `t3_platform.h` from builds and include paths.
2. **T3000 `T3EngineDll`** — Ensure `AdditionalIncludeDirectories` points at `..\T3Engine\include` and the solution builds **Win32** DLL next to `T3000.exe`; confirm `LoadLibrary` / `GetProcAddress` smoke in `InitInstance` in **Debug** when `T3Engine.dll` is present.
3. **Shared product descriptor** — `include/temco_product_desc.h` defines the **Temco product identity** layout and helpers; ESP32 `pro_info` and any PC code that parses the same blob should include this header so **VID/PID** placement and offsets stay in sync.
4. **ESP-IDF** — When more of the core is linked into firmware, keep **portable** translation units free of ESP-IDF headers where possible; supply **host hooks** from `t3_engine.h` (and successors) from the appropriate board/RTOS layer.
5. **`.NET Framework` / csproj bumps** — Keep **target framework** and COM interop changes in **separate PRs** from engine integration (team preference).
6. **Submodule discipline** — Prefer **one canonical T3Engine remote** (e.g. `temcocontrols/T3_Engine`) and pin **SHA** in consuming repos; avoid long-lived drift between a nested `T3Engine/` tree and upstream.
7. **`winPC` product ID (217)** — Downstream **T3000 / ISP** added **`PM_WINPC`** and **`217=winPC`** in product tables. Confirm with **firmware, DB, and tooling** owners that **217** is the agreed ID everywhere (no collision with other registries). This repo does not define numeric product IDs; track cross-team sign-off here so integrators know to verify after each T3Engine/submodule release.

---

## Medium-term (product)

- Flesh out **`t3_engine.h`** with additional hooks (logging, config persistence, I/O abstraction) as the interpreter and scan loop land.
- Add **unit tests** per subsystem (BACnet decode, Modbus framing, routing tables) runnable on the host.
- **Optional compile flags** for features not present on all SKUs (e.g. SNMP only where flash and policy allow).

---

## Questions to revisit later

- Single **DEF/exports** strategy for Windows vs. explicit `__declspec(dllexport)` list as exports grow.
- Whether **Lite** firmware builds link a **subset** of object files or use linker sections / feature macros.
