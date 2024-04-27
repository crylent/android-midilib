#include "Sampler.h"

#include <utility>
#include "../AudioEngine.h"
#include "../NoteFrequency.h"
#include "../Assets.h"

float Sampler::sample(double time, int8_t note) {
    auto& data = mSamples[note];
    if (!data.empty()) {
        auto i = (size_t) round(time / AudioEngine::getTimeIncrement());
        if (i < data.size()) return data[i];
    }
    return NAN;
}

void Sampler::loadSample(int8_t note, vector<uint8_t>& wavData) {
    uint32_t sampleRate;
    vector<float> samples;
    Assets::loadWavData(wavData, samples, sampleRate);
    if (AudioEngine::getSampleRate() != sampleRate) {
        double ratio = double(AudioEngine::getSampleRate()) / double(sampleRate);
        resampleAndAssign(samples, ratio, note);
    } else {
        mSamples[note] = samples;
    }
}

void Sampler::copySampleToRange(int8_t baseNote, int8_t min, int8_t max) {
    for (int8_t note = min; note <= max && note > 0; note++) {
        if (note == baseNote) continue;
        double ratio = NoteFrequency::get(baseNote) / NoteFrequency::get(note);
        resampleAndAssign(mSamples[baseNote], ratio, note);
    }
}

void Sampler::resampleAndAssign(vector<float> &dataIn, double ratio, int8_t note) {
    vector<float> dataOut;
    Assets::resample(dataIn, dataOut, ratio);
    mSamples[note] = dataOut;
}