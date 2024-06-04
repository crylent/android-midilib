#ifndef CUSTOM_OSCILLATOR_H
#define CUSTOM_OSCILLATOR_H

#include "Oscillator.h"
#include "../instrument/Sampler.h"

class CustomOscillator: public Oscillator {
    using Oscillator::Oscillator;

public:
    float evalVoice(double time, float frequency) override;

    CustomOscillator(vector<uint8_t>& wavData,
                     float amplitude = 1, float phase = 0, float freqFactor = 1);
    CustomOscillator(Oscillator &other);

    void loadWaveform(vector<uint8_t>& wavData);

    static void enableLinearInterpolation(bool enable) {
        mLinearInterpolation = enable;
    }

private:
    vector<float> mSamples = vector<float>();
    const size_t mSamplesNum = 1e4;

    static bool mLinearInterpolation;
};


#endif //CUSTOM_OSCILLATOR_H
