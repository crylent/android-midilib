#include "ReleaseOnlyEnvelope.h"
#include <stdexcept>

float ReleaseOnlyEnvelope::envelope(double time, double timeReleased) {
    if (time > timeReleased + mRelease) { // final phase
        return NAN;
    }
    if (time > timeReleased) { // release phase
        return powf(((float) timeReleased + mRelease - (float) time) / mRelease, mReleaseSharpness);
    }
    return 1; // normal phase
}

/**
 * Constructs new Release-only envelope generator.
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
ReleaseOnlyEnvelope::ReleaseOnlyEnvelope(float release) {
    setRelease(release);
}

/**
 * Constructs new Release-only envelope generator.
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @param releaseSharpness release sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
ReleaseOnlyEnvelope::ReleaseOnlyEnvelope(float release, float releaseSharpness) {
    setRelease(release);
    setReleaseSharpness(releaseSharpness);
}


/**
 * Sets <b>release</b> value for ADSR envelope generator.
 * @param release time (in seconds) taken for the level to decay from the sustain level to zero after the key is released
 * @see <a href="https://en.wikipedia.org/wiki/Envelope_(music)">Envelope</a>
 */
void ReleaseOnlyEnvelope::setRelease(float release) {
    if (release < 0) {
        throw std::invalid_argument("Release value must not be negative");
    }
    mRelease = release;
}

/**
 * Sets release sharpness for ADSR envelope generator.
 * @param sharpness release sharpness, 1 is linear, >1 for faster start, \<1 for slower start
 */
void ReleaseOnlyEnvelope::setReleaseSharpness(float sharpness) {
    sharpnessCheck(sharpness);
    mReleaseSharpness = sharpness;
}

