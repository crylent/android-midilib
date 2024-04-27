#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "oboe/Oboe.h"
#include "AudioCallback.h"
#include "player/SoundPlayer.h"
#include "Channel.h"

using namespace std;
using namespace oboe;
using OnStartCallback = function<void()>;

#define AUTO_DEFINITION -1
//#define TEST_LATENCY

class AudioEngine {
public:
    static Result start(SharingMode sharingMode, int32_t sampleRate = AUTO_DEFINITION, int32_t bufferSize = AUTO_DEFINITION);
    static Result start();
    static Result pause();
    static Result resume();
    static Result stop();

    static void initChannels(shared_ptr<Instrument> instrument);

    static bool isShared();
    static int32_t getSampleRate();
    static int32_t getBufferSize();
    static double getTimeIncrement();

    static SoundPlayer& getPlayer();
    static FXList& getMasterFX();

    static vector<unique_ptr<Channel>>& getChannels();
    static uint8_t getNumChannels();

    static void noteOn(int8_t channel, int8_t note, float amplitude);
    static void noteOff(int8_t channel, int8_t note);
    static void allNotesOff(int8_t channel);

    static void AddOnStartListener(OnStartCallback listener);

#ifdef TEST_LATENCY
    static void logLatency();
#endif //TEST_LATENCY

private:
    static unique_ptr<SoundPlayer> mPlayer;
    static shared_ptr<oboe::AudioStream> mStream;
    static mutex mLock;
    static SharingMode mSharingMode;
    static int32_t mSampleRate;
    static double mTimeIncrement;
    static int32_t mBufferSize;

    static shared_ptr<FXList> mMasterEffects;

    static const uint8_t mNumChannels = 16;
    static vector<unique_ptr<Channel>> mChannels;

    static vector<OnStartCallback> mOnStartListeners;

#ifdef TEST_LATENCY
    static bool logDone;
    static chrono::time_point<chrono::steady_clock, chrono::nanoseconds> mNoteOnTime;
#endif //TEST_LATENCY
};

#endif //AUDIO_ENGINE_H
