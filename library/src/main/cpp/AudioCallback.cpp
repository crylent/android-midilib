#include "AudioCallback.h"
#include <utility>
#include "log.h"
#include "AudioEngine.h"

DataCallbackResult AudioCallback::onAudioReady(
        AudioStream *audioStream, void *audioData, int32_t numFrames
        ) {
    auto* floatData = static_cast<float*>(audioData);
    mSoundPlayer.fillBuffer(floatData, numFrames);

#ifdef TEST_LATENCY
    AudioEngine::logLatency();
#endif //TEST_LATENCY

    return DataCallbackResult::Continue;
}