# Architecture

The code is split into a small portable core and a command-line front end.

- `include/rsi.h`: public data structures and function contracts.
- `src/system.c`: OS, architecture, processor count, and supported memory metrics.
- `src/disk.c`: filesystem capacity using read-only OS APIs.
- `src/analyze.c`: streaming byte/line/word counter.
- `src/hash.c`: streaming FNV-1a 64-bit fingerprint.
- `src/main.c`: argument parsing and output.

The implementation uses C17 and avoids shell execution. Platform-specific code is isolated behind compile-time checks. Unsupported metrics are reported as unavailable rather than fabricated.
