#include "Instrument.h"

Instrument::Instrument(unique_ptr<Envelope> envelope) {
    mEnvelope = std::move(envelope);
}

float Instrument::eval(double time, int8_t note, double timeReleased) {
    float env = mEnvelope->envelope(time, timeReleased);
    return sample(time, note) * env;
}

Envelope& Instrument::getEnvelope() {
    return *mEnvelope;
}
