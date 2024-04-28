#ifndef FX_LIST_H
#define FX_LIST_H

#include "soundfx/SoundFX.h"
#include <memory>
#include <vector>

using namespace std;

class FXList {
public:
    uint8_t addEffect(unique_ptr<SoundFX> fx);
    uint8_t insertEffect(unique_ptr<SoundFX> fx, uint8_t i);
    void moveEffect(uint8_t i, uint8_t newIndex);
    void removeEffect(uint8_t i);
    SoundFX& getEffect(uint8_t i);
    void clearEffects();

    float applyFX(float sample);

private:
    vector<unique_ptr<SoundFX>> mEffects = vector<unique_ptr<SoundFX>>();
};


#endif //FX_LIST_H
