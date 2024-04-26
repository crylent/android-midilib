#ifndef ADSR_ENVELOPE_H
#define ADSR_ENVELOPE_H

#include "ReleaseOnlyEnvelope.h"

class ADSREnvelope: public ReleaseOnlyEnvelope {
public:
    ADSREnvelope() {}
    ADSREnvelope(float attack, float decay, float sustain, float release);
    ADSREnvelope(float attack, float decay, float sustain, float release,
               float attackSharpness, float decaySharpness, float releaseSharpness);

    void setEnvelope(float attack, float decay, float sustain, float release);
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setEnvelopeSharpness(float attack, float decay, float release);
    void setAttackSharpness(float sharpness);
    void setDecaySharpness(float sharpness);

    float envelope(double time, double timeReleased) override;

private:
    float mAttack = 0; // seconds (after noteOn)
    float mDecay = 0; // seconds (after noteOn)
    float mSustain = 1; // from 0.0 to 1.0

    float mAttackSharpness = 1;
    float mDecaySharpness = 1;
};


#endif //ADSR_ENVELOPE_H
