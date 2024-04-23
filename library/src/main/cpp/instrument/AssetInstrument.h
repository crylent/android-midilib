#ifndef ASSET_INSTRUMENT_H
#define ASSET_INSTRUMENT_H

#include <vector>
#include "Instrument.h"

using namespace std;

class AssetInstrument : public Instrument {
public:
    using Instrument::Instrument;

    void loadAsset(int8_t note, vector<uint8_t>& wavData);
    void copySampleToRange(int8_t baseNote, int8_t min, int8_t max);

    float sample(double time, int8_t note) override;

private:
    vector<vector<float>> mSamples = vector<vector<float>>(SCHAR_MAX + 1);

    void resampleAndAssign(vector<float>& dataIn, double ratio, int8_t note);
};


#endif //ASSET_INSTRUMENT_H
