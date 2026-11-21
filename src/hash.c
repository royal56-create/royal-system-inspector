#include "rsi.h"

int rsi_hash_file_fnv1a64(const char *path, uint64_t *hash_out) {
    if (!path || !hash_out) return -1;
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;
    uint64_t hash = UINT64_C(14695981039346656037);
    const uint64_t prime = UINT64_C(1099511628211);
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        hash ^= (unsigned char)ch;
        hash *= prime;
    }
    if (ferror(fp)) { fclose(fp); return -1; }
    fclose(fp);
    *hash_out = hash;
    return 0;
}
