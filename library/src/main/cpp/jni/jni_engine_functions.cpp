#include "../player/SoundPlayer.h"

static void renderNSamples(SoundPlayer& player, size_t bufferSize, vector<float>& samples) {
    auto* buffer = new float[bufferSize];
    player.fillBuffer(buffer, bufferSize);
    samples.insert(samples.end(), buffer, buffer + bufferSize - 1);
}