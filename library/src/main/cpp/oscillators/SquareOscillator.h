#ifndef SQUARE_OSCILLATOR_H
#define SQUARE_OSCILLATOR_H

#include "Oscillator.h"

class SquareOscillator: public Oscillator {
    using Oscillator::Oscillator;

    float evalVoice(double time, float frequency) override;

public:
    SquareOscillator(Oscillator &other);
};


#endif //SQUARE_OSCILLATOR_H
