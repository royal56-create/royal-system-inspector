# Architecture

Royal System Inspector is a C17 command-line application organized into a small system-information library and a CLI.

- `include/royal_inspector/system_info.h`: shared data model and function declarations.
- `src/system_info.c`: platform-specific collection of OS, host, CPU, memory, disk, uptime, and process-count information.
- `src/report.c`: JSON, CSV, and plain-text report writers.
- `src/main.c`: command parsing and terminal output.
- `tests/test_system.c`: smoke test for system-information collection.
- `CMakeLists.txt`: build, test, and install configuration.
- `.github/workflows/ci.yml`: Ubuntu and Windows build/test workflow.

Linux uses read-only interfaces such as `/proc`, `uname`, and `statvfs`. Windows uses documented system APIs, including GlobalMemoryStatusEx, GetDiskFreeSpaceEx, and Toolhelp process enumeration. The initial release reports only metrics actually collected; unsupported metrics are explicitly identified rather than estimated.
