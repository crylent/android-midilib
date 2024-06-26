#include "Wave.h"

#include <utility>
#include "AudioEngine.h"

Wave::Wave(Instrument &instrument, int8_t note, float amplitude) : mInstrument(instrument) {
    mNote = note;
    mAmplitude = amplitude;
    mTimeIncrement = AudioEngine::getTimeIncrement();
}

float Wave::nextSample() {
    float val = mInstrument.eval(mTime, mNote, mTimeReleased);
    mTime += mTimeIncrement;
    return val * mAmplitude;
}

void Wave::release() {
    mTimeReleased = mTime;
}
