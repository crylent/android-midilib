#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "oboe/Oboe.h"
#include "../soundfx/SoundFX.h"
#include "../FXList.h"

using namespace std;

class SoundPlayer {
public:
    void fillBuffer(float* buffer, size_t numFrames);

    shared_ptr<FXList> getEffects();

private:
    float applyFX(float sample);

    shared_ptr<FXList> mEffects = make_shared<FXList>();
};


#endif //SOUND_PLAYER_H
