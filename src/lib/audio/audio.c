#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_engine engine;

int audio_init(void)
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return -1;
    }
    return 0;
}

int audio_play(const char *filename)
{
    ma_result result = ma_engine_play_sound(&engine, filename, NULL);
    if (result != MA_SUCCESS) {
        return -1;
    }
    return 0;
}
