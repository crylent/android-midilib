#ifndef RELEASE_ONLY_ENVELOPE_H
#define RELEASE_ONLY_ENVELOPE_H

#include "Envelope.h"

/**
 * Recommended for Sampler
 */
class ReleaseOnlyEnvelope: public Envelope {
public:
    ReleaseOnlyEnvelope() {}
    ReleaseOnlyEnvelope(float release);
    ReleaseOnlyEnvelope(float release, float releaseSharpness);

    void setRelease(float release);
    void setReleaseSharpness(float sharpness);

    float envelope(double time, double timeReleased) override;

protected:
    float mRelease = 0; // seconds (after noteOff)
    float mReleaseSharpness = 1;
};


#endif //RELEASE_ONLY_ENVELOPE_H
