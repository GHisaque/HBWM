#pragma once

#ifndef GIP_AUDIO_H
#define GIP_AUDIO_H

#include <windows.h>
#include <mmeapi.h>

// master volume only
static inline void Audio_Set_Volume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    WORD channel_vol = (WORD)(volume * 65535.0f);
    DWORD combined_vol = ((DWORD)channel_vol << 16) | channel_vol;

    waveOutSetVolume(NULL, combined_vol);
}

// plays wav only
static inline void Audio_Play(const char* path, bool loop) {
    if (!path) return;
    if (loop) PlaySoundA(path, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
    else PlaySoundA(path, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

// stops audio globally
static inline void Audio_Stop(void) {
    PlaySoundA(NULL, NULL, 0);
}

#endif