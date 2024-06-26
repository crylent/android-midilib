#include "CustomOscillator.h"
#include "../third_party/AudioFile.h"
#include "../AudioEngine.h"
#include "../Assets.h"
#include "../NoteFrequency.h"
#include "../log.h"

bool CustomOscillator::mLinearInterpolation = false;

float CustomOscillator::evalVoice(double time, float frequency) {
    if (mSamples.empty()) return sinf(calcPhase(time, frequency));
    float fractionPhase = calcFractionPhase(time, frequency);
    float point = fractionPhase * mSamplesNum;
    size_t i = size_t(point);
    float left = mSamples[i];
    if (!mLinearInterpolation) return left; // point interpolation

    float right;
    if (i + 1 < mSamplesNum) right = mSamples.at(i + 1);
    else right = mSamples.at(0);
    float integral;
    float fractional = modf(point, &integral);
    return left + (right - left) * fractional; // linear interpolation
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
    Assets::resample(samples, mSamples, mSamplesNum);
}
