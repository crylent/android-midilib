#include "ReverseSawtoothOscillator.h"

float ReverseSawtoothOscillator::evalVoice(double time, float frequency) {
    float p = calcPhase(time, frequency);
    return float(-p / M_PI);
}

ReverseSawtoothOscillator::ReverseSawtoothOscillator(Oscillator &other) {
    copyFrom(other);
}
