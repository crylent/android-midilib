#include "Channel.h"
#include <utility>
#include "NoteFrequency.h"

Channel::Channel(shared_ptr<Instrument> instrument) {
    mInstrument = instrument;
}

void Channel::setInstrument(shared_ptr<Instrument> instrument) {
    mInstrument = std::move(instrument);
}

float Channel::nextSample() {
    lock_guard<mutex> lockGuard(mLock);
    float sampleValue = 0;
    for (auto & wave : mWaves) {
        float waveValue = wave.second->nextSample();
        if (isnan(waveValue)) {
            mWaves.erase(wave.first);
        } else {
            sampleValue += waveValue;
        }
    }
    return sampleValue;
}

void Channel::noteOn(int8_t note, float amplitude) {
    lock_guard<mutex> lockGuard(mLock);
    auto wave = make_unique<Wave>(*mInstrument, note, amplitude);
    mWaves[note] = std::move(wave);
}

void Channel::noteOff(int8_t note) {
    lock_guard<mutex> lockGuard(mLock);
    auto wave = mWaves.find(note);
    if (wave != mWaves.end()) {
        wave->second->release();
    }
}

void Channel::allNotesOff() {
    lock_guard<mutex> lockGuard(mLock);
    for (auto & wave : mWaves) {
        wave.second->release();
    }
}
