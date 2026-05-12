# Build smoke test on Windows: finds WinGet-installed WinLibs gcc if not on PATH.
$ErrorActionPreference = "Stop"
$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not (Test-Path (Join-Path $repoRoot "include\t3_platform.h"))) {
    throw "Expected include\t3_platform.h under $repoRoot"
}

function Find-WinLibsGcc {
    $roots = @(
        "$env:LOCALAPPDATA\Microsoft\WinGet\Packages",
        "$env:ProgramFiles\WinLibs*"
    )
    foreach ($root in $roots) {
        if (-not (Test-Path $root)) { continue }
        $gcc = Get-ChildItem -Path $root -Filter "gcc.exe" -Recurse -ErrorAction SilentlyContinue |
            Where-Object { $_.FullName -match "mingw64\\bin\\gcc\.exe$" } |
            Select-Object -First 1
        if ($gcc) { return $gcc.FullName }
    }
    return $null
}

$gcc = $null
try { $gcc = (Get-Command gcc -ErrorAction Stop).Source } catch { }

if (-not $gcc) {
    $gcc = Find-WinLibsGcc
}

if (-not $gcc) {
    Write-Error "gcc not found. Install with: winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT"
}

$mingwBin = Split-Path -Parent $gcc
if (Test-Path $mingwBin) {
    $env:Path = "$mingwBin;$env:Path"
}

Write-Host "Using: $gcc"
Push-Location $repoRoot
try {
    & $gcc -std=c11 -Wall -Wextra -Werror -Iinclude -D_DEFAULT_SOURCE `
        tests/smoke_main.c src/t3_clock.c -o smoke.exe
    & (Join-Path $repoRoot "smoke.exe")
} finally {
    Pop-Location
}
