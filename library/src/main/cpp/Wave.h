#ifndef WAVE_H
#define WAVE_H

#include "instrument/Instrument.h"

using namespace std;

class Wave {
public:
    Wave(Instrument& instrument, int8_t note, float amplitude);

    float nextSample();

    void release();

private:
    Instrument& mInstrument;
    int8_t mNote;
    float mAmplitude;
    double mTimeIncrement;

    double mTime = 0;
    double mTimeReleased = INFINITY;
};


#endif //WAVE_H
