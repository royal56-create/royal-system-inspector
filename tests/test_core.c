#include "rsi.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static int write_fixture(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fputs("one two\nthree\n", f);
    return fclose(f);
}

int main(void) {
    const char *path = "rsi_test_fixture.tmp";
    RsiFileStats stats;
    uint64_t hash1, hash2;
    if (write_fixture(path) != 0) return 1;
    if (rsi_analyze_file(path, &stats) != 0 ||
        stats.bytes != 14 || stats.lines != 2 || stats.words != 3) {
        remove(path); fprintf(stderr, "analyze test failed\n"); return 1;
    }
    if (rsi_hash_file_fnv1a64(path, &hash1) != 0 ||
        rsi_hash_file_fnv1a64(path, &hash2) != 0 || hash1 != hash2) {
        remove(path); fprintf(stderr, "hash determinism test failed\n"); return 1;
    }
    remove(path);
    puts("All core tests passed.");
    return 0;
}
