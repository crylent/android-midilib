#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "oboe/Oboe.h"
#include "../envelope/Envelope.h"

using namespace std;

class Instrument {
public:
    Instrument(unique_ptr<Envelope> envelope);
    virtual ~Instrument() = default;

    float eval(double time, int8_t note, double timeReleased);

    Envelope& getEnvelope();

protected:
    virtual float sample(double time, int8_t note) = 0;

    unique_ptr<Envelope> mEnvelope;
};


#endif //INSTRUMENT_H
