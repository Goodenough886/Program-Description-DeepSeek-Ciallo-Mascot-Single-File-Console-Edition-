# Ciallo Mascot · Console Edition

A tiny, **single-file C++17** console mascot: a five-pointed star drawn with `*` characters, with a cheerful face in the center.

**中文简介：** 一个无需第三方依赖的 C++17 控制台小程序。自动适配终端尺寸，用字符画展示带表情的五角星，也可重现原始的 448 × 250 超大画布。

## Quick start / 快速开始

Requires a C++17 compiler (GCC, Clang, or MSVC).

**Linux / macOS:**

```sh
c++ -std=c++17 -O2 -Wall -Wextra ciallo.cpp -o ciallo
./ciallo
```

**Windows (MinGW-w64 g++):**

```powershell
g++ -std=c++17 -O2 -Wall -Wextra ciallo.cpp -o ciallo.exe
.\ciallo.exe
```

**Windows (Visual Studio Developer Command Prompt):**

```cmd
cl /std:c++17 /EHsc ciallo.cpp
ciallo.exe
```

## Options / 参数

| Option | Description |
| --- | --- |
| (none) | Detect terminal size, keep the drawing at most 88 × 32 by default. |
| `--width N --height N` | Set canvas dimensions (width 24–512, height 12–300). |
| `--original` | Render the original 448 × 250 canvas. **This will scroll on a typical terminal.** |
| `--color` | Use ANSI yellow (requires a terminal with ANSI color support). |
| `--beep` | Request an optional terminal bell. Its sound depends on terminal/OS settings. |
| `--help` | Show command-line usage. |

Examples:

```sh
./ciallo --width 72 --height 26
./ciallo --color --beep
./ciallo --original
```

On Windows, replace `./ciallo` with `.\ciallo.exe`.

## Design notes / 设计说明

- The star is computed from a ten-vertex polygon (alternating outer and inner radii), rather than storing a gigantic fixed ASCII-art file.
- The drawing uses an approximate terminal character-cell aspect ratio, so the shape is less distorted across different canvas sizes.
- A small face is added at the center of the star; no external graphics library is required.
- **Voice is not implemented.** The original concept mentioned a spoken “Ciallo” effect via the PC speaker, but the repository does not contain a licensed recording or a speech synthesizer. `--beep` is a terminal notification bell, **not a recorded voice**.
- Output is plain ASCII by default. The program can be compiled on Windows, Linux, and macOS; the bell and ANSI color depend on terminal support.

## Project status

This repository started as a program description only. The implementation is now available as [`ciallo.cpp`](ciallo.cpp). Contributions and improvements to facial expressions, actual voice playback (with an appropriately licensed audio asset), and terminal compatibility are welcome.
