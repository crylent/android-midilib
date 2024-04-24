#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "oboe/Oboe.h"
#include "../soundfx/SoundFX.h"
#include "../FXList.h"

using namespace std;

class SoundPlayer {
public:
    virtual ~SoundPlayer() = default;

    virtual void fillBuffer(float* buffer, size_t numFrames) = 0;

    shared_ptr<FXList> getEffects();

protected:
    float applyFX(float sample);

private:
    shared_ptr<FXList> mEffects = make_shared<FXList>();
};


#endif //SOUND_PLAYER_H
