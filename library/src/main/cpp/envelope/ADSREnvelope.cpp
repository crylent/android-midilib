#include "ADSREnvelope.h"
#include <stdexcept>

float ADSREnvelope::envelope(double time, double timeReleased) {
    if (time > timeReleased + mRelease) { // final phase
        return NAN;
    }
    if (time > timeReleased) { // release phase
        float ampWhenReleased;
        if (timeReleased < mAttack) { // released in attack phase
            ampWhenReleased = powf(float(timeReleased) / mAttack, 1.0f / mAttackSharpness);
        } else if (time < mAttack + mDecay) { // released in decay phase
            ampWhenReleased = powf((mAttack + mDecay - (float) timeReleased) / mDecay, mDecaySharpness) * (1 - mSustain) + mSustain;
        } else { // released in sustain phase
            ampWhenReleased = mSustain;
        }
        return powf(((float) timeReleased + mRelease - (float) time) / mRelease, mReleaseSharpness) * ampWhenReleased;
    }
    if (time < mAttack) { // attack phase
        return powf(float(time) / mAttack, 1.0f / mAttackSharpness);
    }
    if (time < mAttack + mDecay) { // decay phase
        return powf((mAttack + mDecay - (float) time) / mDecay, mDecaySharpness) * (1 - mSustain) + mSustain;
    }
    return mSustain; // sustain phase
}

/**
 * Constructs new ADSR envelope generator.
 * @param attack time (in seconds) taken for initial run-up of level from nil to peak, beginning when the key is pressed
 * @param decay time (in seconds) taken for the subsequent run down from the attack level to the designated sustain level
 * @param sustain level (from 0.0 to 1.0) during the main sequence of the sound's duration, until the key is released
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
ADSREnvelope::ADSREnvelope(float attack, float decay, float sustain, float release) {
    setEnvelope(attack, decay, sustain, release);
}

/**
 * Constructs new ADSR envelope generator.
 * @param attack time (in seconds) taken for initial run-up of level from nil to peak, beginning when the key is pressed
 * @param decay time (in seconds) taken for the subsequent run down from the attack level to the designated sustain level
 * @param sustain level (from 0.0 to 1.0) during the main sequence of the sound's duration, until the key is released
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @param attackSharpness attack sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @param decaySharpness decay sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @param releaseSharpness release sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
ADSREnvelope::ADSREnvelope(float attack, float decay, float sustain, float release,
                       float attackSharpness, float decaySharpness, float releaseSharpness) {
    setEnvelope(attack, decay, sustain, release);
    setEnvelopeSharpness(attackSharpness, decaySharpness, releaseSharpness);
}


/**
 * Sets all parameters for ADSR envelope generator.
 * @param attack time (in seconds) taken for initial run-up of level from nil to peak, beginning when the key is pressed
 * @param decay time (in seconds) taken for the subsequent run down from the attack level to the designated sustain level
 * @param sustain level (from 0.0 to 1.0) during the main sequence of the sound's duration, until the key is released
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
void ADSREnvelope::setEnvelope(float attack, float decay, float sustain, float release) {
    setAttack(attack);
    setDecay(decay);
    setSustain(sustain);
    setRelease(release);
}

/**
 * Sets <b>attack</b> value for ADSR envelope generator.
 * @param attack time (in seconds) taken for initial run-up of level from nil to peak, beginning when the key is pressed
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
void ADSREnvelope::setAttack(float attack) {
    if (attack < 0) {
        throw std::invalid_argument("Attack value must not be negative");
    }
    mAttack = attack;
}
/**
 * Sets <b>decay</b> value for ADSR envelope generator.
 * @param decay time (in seconds) taken for the subsequent run down from the attack level to the designated sustain level
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
void ADSREnvelope::setDecay(float decay) {
    if (decay < 0) {
        throw std::invalid_argument("Decay value must not be negative");
    }
    mDecay = decay;
}

/**
 * Sets <b>sustain</b> value for ADSR envelope generator.
 * @param sustain level (from 0.0 to 1.0) during the main sequence of the sound's duration, until the key is released
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
void ADSREnvelope::setSustain(float sustain) {
    if (sustain < 0 || sustain > 1) {
        throw std::invalid_argument("Sustain value must be in the range of 0.0 to 1.0");
    }
    mSustain = sustain;
}

/**
 * Sets sharpness parameters for ADSR envelope generator.
 * @param attack attack sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @param decay decay sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @param release release sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 */
void ADSREnvelope::setEnvelopeSharpness(float attack, float decay, float release) {
    setAttackSharpness(attack);
    setDecaySharpness(decay);
    setReleaseSharpness(release);
}

/**
 * Sets attack sharpness for ADSR envelope generator.
 * @param sharpness attack sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 */
void ADSREnvelope::setAttackSharpness(float sharpness) {
    sharpnessCheck(sharpness);
    mAttackSharpness = sharpness;
}

/**
 * Sets decay sharpness for ADSR envelope generator.
 * @param sharpness decay sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 */
void ADSREnvelope::setDecaySharpness(float sharpness) {
    sharpnessCheck(sharpness);
    mDecaySharpness = sharpness;
}