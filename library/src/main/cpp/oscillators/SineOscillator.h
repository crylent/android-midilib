#ifndef SINE_OSCILLATOR_H
#define SINE_OSCILLATOR_H

#include "Oscillator.h"

class SineOscillator: public Oscillator {
    using Oscillator::Oscillator;

    float evalVoice(double time, float frequency) override;

public:
    SineOscillator(Oscillator &other);
};


#endif //SINE_OSCILLATOR_H
