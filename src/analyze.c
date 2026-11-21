#include "rsi.h"
#include <ctype.h>

int rsi_analyze_file(const char *path, RsiFileStats *stats) {
    if (!path || !stats) return -1;
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;
    stats->bytes = stats->lines = stats->words = 0;
    int ch, in_word = 0, last = EOF;
    while ((ch = fgetc(fp)) != EOF) {
        stats->bytes++;
        if (ch == '\n') stats->lines++;
        if (isspace((unsigned char)ch)) in_word = 0;
        else if (!in_word) { stats->words++; in_word = 1; }
        last = ch;
    }
    if (ferror(fp)) { fclose(fp); return -1; }
    if (stats->bytes > 0 && last != '\n') stats->lines++;
    fclose(fp);
    return 0;
}
