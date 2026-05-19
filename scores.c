#include "scores.h"

void scores_load(ScoreBoard *sb) {
    for (int i = 0; i < SCORES_COUNT; i++) sb->scores[i] = 0;
    FILE *f = fopen(SCORES_FILE, "rb");
    if (!f) return;
    fread(sb->scores, sizeof(int), SCORES_COUNT, f);
    fclose(f);
}

void scores_save(ScoreBoard *sb, int new_score) {
    sb->scores[SCORES_COUNT-1] = new_score;
    for (int i = SCORES_COUNT-1; i > 0; i--) {
        if (sb->scores[i] > sb->scores[i-1]) {
            int tmp = sb->scores[i];
            sb->scores[i]   = sb->scores[i-1];
            sb->scores[i-1] = tmp;
        } else break;
    }
    FILE *f = fopen(SCORES_FILE, "wb");
    if (!f) return;
    fwrite(sb->scores, sizeof(int), SCORES_COUNT, f);
    fclose(f);
}
