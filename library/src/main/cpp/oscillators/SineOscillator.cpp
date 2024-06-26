#include "SineOscillator.h"

float SineOscillator::evalVoice(double time, float frequency) {
    return sinf(calcPhase(time, frequency));
}

SineOscillator::SineOscillator(Oscillator &other) {
    copyFrom(other);
}
