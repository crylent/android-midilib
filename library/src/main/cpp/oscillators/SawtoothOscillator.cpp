#include "SawtoothOscillator.h"

float SawtoothOscillator::evalVoice(double time, float frequency) {
    float p = calcPhase(time, frequency);
    return float(p / M_PI);
}

SawtoothOscillator::SawtoothOscillator(Oscillator &other) {
    copyFrom(other);
}
