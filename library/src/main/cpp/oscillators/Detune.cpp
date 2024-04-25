#include "Detune.h"
#include "Oscillator.h"

/**
 * Constructs new Detune object.
 * @param owner reference to oscillator object
 * @param unisonVoices number of unison voices, 2 or more
 * @param detune maximum divergence from the original frequency in octaves
 */
Detune::Detune(Oscillator& owner, uint8_t unisonVoices, float detune) : mOwner(owner) {
    setUnisonVoices(unisonVoices);
    setDetune(detune);
}

/**
 * Constructs new Detune object from other
 * @param owner reference to oscillator object
 * @param other other detune object
 */
Detune::Detune(Oscillator &owner, Detune &other) : mOwner(owner) {
    mUnisonVoices = other.mUnisonVoices;
    mDetune = other.mDetune;
}

float Detune::process(double time, float frequency) {
    float sum = 0;
    float detuneDelta = 2 * mDetune / mUnisonVoices;
    for (uint8_t i = 0; i < mUnisonVoices; i++) {
        sum += mOwner.evalVoice(time, frequency * powf(2, -mDetune + detuneDelta * i));
    }
    return sum / mUnisonVoices;
}

/**
 * Sets number of unison voices for detune.
 * @param unisonVoices number of unison voices, 2 or more
 * @see <a href="https://samplechilli.com/what-is-synth-detune-and-why-use-it/">Detune</a>
 */
Detune& Detune::setUnisonVoices(uint8_t unisonVoices) {
    if (unisonVoices < 2) {
        throw invalid_argument("You should set 2 or more unison voices");
    }
    mUnisonVoices = unisonVoices;
    return *this;
}

/**
 * Sets detune level.
 * @param detune maximum divergence from the original frequency in octaves
 * @see <a href="https://samplechilli.com/what-is-synth-detune-and-why-use-it/">Detune</a>
 */
Detune& Detune::setDetune(float detune) {
    if (detune <= 0) {
        throw invalid_argument("Detune must be positive number");
    }
    mDetune = detune;
    return *this;
}