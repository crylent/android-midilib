#include "Synthesizer.h"
#include "../NoteFrequency.h"

float Synthesizer::sample(double time, int8_t note) {
    float value = 0;
    float frequency = NoteFrequency::get(note);
    for (auto & oscillator : mOscillators) {
        value += oscillator->eval(time, frequency);
    }
    return value;
}

void Synthesizer::addOscillator(unique_ptr<Oscillator> oscillator) {
    mOscillators.push_back(std::move(oscillator));
}

Oscillator& Synthesizer::getOscillatorByIndex(uint8_t index) {
    return *mOscillators[index];
}

void Synthesizer::enableOscillator(uint8_t index) {
    mOscillators[index]->enable();
}

void Synthesizer::disableOscillator(uint8_t index) {
    mOscillators[index]->disable();
}

void Synthesizer::removeOscillator(uint8_t index) {
    mOscillators.erase(mOscillators.begin() + index);
}

template<typename Shape> requires(is_base_of<Oscillator, Shape>() == true)
void Synthesizer::setOscillatorShape(uint8_t index) {
    // construct oscillator with the same parameters (using copyFrom method)
    mOscillators[index] = make_unique<Shape>(*mOscillators[index]);
}
