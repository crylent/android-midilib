#include "SoundPlayer.h"
#include "../AudioEngine.h"

void SoundPlayer::fillBuffer(float *buffer, size_t numFrames) {
    for (int32_t i = 0; i < numFrames; i++) {
        float sampleValue = 0;
        for (auto & channel : AudioEngine::getChannels()) {
            sampleValue += channel->nextSample();
        }
        buffer[i] = applyFX(sampleValue);
    }
}

float SoundPlayer::applyFX(float sample) {
    return mEffects->applyFX(sample);
}

shared_ptr<FXList> SoundPlayer::getEffects() {
    return mEffects;
}
