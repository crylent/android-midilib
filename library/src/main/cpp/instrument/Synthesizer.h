#ifndef WAVE_SYNTH_H
#define WAVE_SYNTH_H

#include "Instrument.h"
#include "../oscillators/Oscillator.h"
#include "../oscillators/SineOscillator.h"

using namespace std;

class Synthesizer : public Instrument {
public:
    using Instrument::Instrument;

    void addOscillator(unique_ptr<Oscillator> oscillator);
    Oscillator& getOscillatorByIndex(uint8_t index);
    void enableOscillator(uint8_t index);
    void disableOscillator(uint8_t index);
    void removeOscillator(uint8_t index);

    template<typename Shape> requires(is_base_of<Oscillator, Shape>() == true)
    void setOscillatorShape(uint8_t index) {
        // construct oscillator with the same parameters (using copyFrom method)
        mOscillators[index] = make_unique<Shape>(*mOscillators[index]);
    }

protected:
    float sample(double time, int8_t note) override;

private:
    vector<unique_ptr<Oscillator>> mOscillators = vector<unique_ptr<Oscillator>>();
};

#endif //WAVE_SYNTH_H
