#include "Assets.h"
#include "third_party/AudioFile.h"

uint8_t Assets::mResamplingQuality = SRC_SINC_FASTEST;

void Assets::resample(vector<float> &dataIn, vector<float> &dataOut, size_t targetSamples) {
    double ratio = double(targetSamples) / dataIn.size();
    resample(dataIn, dataOut, ratio, targetSamples);
}

void Assets::resample(vector<float>& dataIn, vector<float>& dataOut, double ratio) {
    size_t outSamples = ceil(double(dataIn.size()) * ratio);
    resample(dataIn, dataOut, ratio, outSamples);
}

void Assets::setResamplingQuality(uint8_t quality) {
    if (quality != SRC_LINEAR &&
        quality != SRC_ZERO_ORDER_HOLD &&
        quality != SRC_SINC_FASTEST &&
        quality != SRC_SINC_MEDIUM_QUALITY &&
        quality != SRC_SINC_BEST_QUALITY) throw invalid_argument("Invalid resampling quality");
    mResamplingQuality = quality;
}

void Assets::resample(vector<float>& dataIn, vector<float>& dataOut,
                      double ratio, size_t outSamples) {
    size_t inSamples = dataIn.size();
    auto data = make_unique<SRC_DATA>();
    data->data_in = dataIn.data();
    auto* outData = new float[outSamples];
    data->data_out = outData;
    data->input_frames = inSamples;
    data->output_frames = outSamples;
    data->src_ratio = ratio;
    src_simple(data.get(), mResamplingQuality, 1);
    dataOut.assign(outData, outData + outSamples);
}

void Assets::loadWavData(vector<uint8_t>& wavData, vector<float>& outSamples, uint32_t& sampleRate) {
    try { // Assume WAV file has float format
        AudioFile<float> audioFile;
        audioFile.loadFromMemory(wavData);
        outSamples = audioFile.samples[0];
        sampleRate = audioFile.getSampleRate();
    } catch (exception& e) { // Integer (PCM) format
        AudioFile<int32_t> audioFile;
        audioFile.loadFromMemory(wavData);
        size_t numSamples = audioFile.getNumSamplesPerChannel();
        outSamples.reserve(numSamples);
        auto sampleMax = (float) pow(2, audioFile.getBitDepth());
        for (size_t i = 0; i < numSamples; i++) {
            outSamples[i] = float(audioFile.samples[0][i]) / sampleMax;
        }
        sampleRate = audioFile.getSampleRate();
    }
}
