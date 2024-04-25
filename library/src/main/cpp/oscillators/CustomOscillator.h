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

private:
    unique_ptr<vector<float>> mSamples = make_unique<vector<float>>();
    const size_t mSamplesNum = 1e5;
};


#endif //CUSTOM_OSCILLATOR_H
