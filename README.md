# Royal System Inspector

A C17 command-line utility for inspecting basic system information and writing local diagnostic reports.

## Implemented features
- OS name, hostname and architecture
- Linux CPU model and logical processor count
- Physical memory total and available memory
- Linux root filesystem or Windows C: drive capacity
- Visible process count
- Basic memory and disk usage indicators
- JSON, CSV and TXT report export
- CMake build, CTest smoke test and Ubuntu/Windows CI workflow

## Known limitations
Network interface enumeration, per-process details, CPU utilization sampling, temperature sensors, Windows CPU model lookup and multi-volume disk enumeration are not implemented in this initial release. The network command states this rather than inventing data. Metrics depend on OS permissions and available APIs.

## Safety
Read-only informational utility. It does not delete files, terminate processes, change settings, capture network traffic or collect credentials.

## Requirements
CMake 3.16+ and a C17 compiler (GCC, Clang or MSVC). Linux and Windows are targeted.

## Build
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## Usage
```sh
./build/royal-inspector overview
./build/royal-inspector cpu
./build/royal-inspector memory
./build/royal-inspector disk
./build/royal-inspector processes
./build/royal-inspector network
./build/royal-inspector diagnostics
./build/royal-inspector report --format json
```

Reports are written to the current directory as system-report.json, system-report.csv or system-report.txt. On Windows, the executable may be under build/Release.

## License
MIT. See LICENSE.
