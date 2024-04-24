#include "CustomOscillator.h"
#include "../third_party/AudioFile.h"
#include "../AudioEngine.h"
#include "../Assets.h"
#include "../NoteFrequency.h"

float CustomOscillator::evalVoice(double time, float frequency) {
    if (!mSamples) return sinf(calcPhase(time, frequency));
    float fractionPhase = calcFractionPhase(time, frequency);
    float point = fractionPhase * mSamplesNum;
    size_t i = size_t(point);
    float left = mSamples->at(i), right;
    if (i + 1 < mSamplesNum) right = mSamples->at(i + 1);
    else right = mSamples->at(0);
    float interPoint = remainderf(point, 1);
    return left + (right - left) * interPoint; // linear interpolation
}

CustomOscillator::CustomOscillator(Oscillator &other) {
    copyFrom(other);
}

CustomOscillator::CustomOscillator(vector<uint8_t>& wavData,
                                   float amplitude, float phase, float freqFactor) :
                                   CustomOscillator(amplitude, phase, freqFactor) {
    loadWaveform(wavData);
}

void CustomOscillator::loadWaveform(vector<uint8_t>& wavData) {
    uint32_t sampleRate;
    vector<float> samples;
    Assets::loadWavData(wavData, samples, sampleRate);
    Assets::resample(samples, *mSamples, mSamplesNum);
}
