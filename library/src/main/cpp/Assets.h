#ifndef RESAMPLING_H
#define RESAMPLING_H

#include <vector>
#include "samplerate.h"

using namespace std;

class Assets {
public:
    static void loadWavData(vector<uint8_t>& wavData, vector<float>& outSamples, uint32_t& sampleRate);

    static void resample(vector<float>& dataIn, vector<float>& dataOut, size_t targetSamples);
    static void resample(vector<float>& dataIn, vector<float>& dataOut, double ratio);
    static void setResamplingQuality(uint8_t quality);

private:
    static void resample(vector<float>& dataIn, vector<float>& dataOut, double ratio, size_t outSamples);

    static uint8_t mResamplingQuality;
};


#endif //RESAMPLING_H
