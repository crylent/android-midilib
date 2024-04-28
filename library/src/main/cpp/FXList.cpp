#include "FXList.h"

/**
 * Adds effect to list.
 * @param fx effect to be applied
 * @return position of effect in the effects list
 */
uint8_t FXList::addEffect(unique_ptr<SoundFX> fx) {
    mEffects.push_back(std::move(fx));
    return mEffects.size() - 1;
}

/**
 * Adds effect to list
 * @param fx effect to be applied
 * @param i position of effect in the effects list
 * @return position of effect in the effects list (can be different if provided <code>i</code> is out of bounds)
 */
uint8_t FXList::insertEffect(unique_ptr<SoundFX> fx, uint8_t i) {
    if (i > mEffects.size()) return addEffect(std::move(fx));
    mEffects.insert(mEffects.begin() + i, std::move(fx));
    return i;
}

/**
 * Moves effect to another position in list
 * @param i position of effect in the effects list
 * @param newIndex new position of effect in the effects list
 */
void FXList::moveEffect(uint8_t i, uint8_t newIndex) {
    mEffects.insert(mEffects.begin() + newIndex, std::move(mEffects[i]));
    mEffects.erase(mEffects.begin() + i);
}

/**
 * Removes effect from list
 * @param i position of effect in the effects list
 */
void FXList::removeEffect(uint8_t i) {
    mEffects.erase(mEffects.begin() + i);
}

/**
 * Removes all effects from list.
 */
void FXList::clearEffects() {
    mEffects.clear();
}

SoundFX &FXList::getEffect(uint8_t i) {
    return *mEffects[i];
}

float FXList::applyFX(float sample) {
    for (auto & fx : mEffects) {
        if (fx->isEnabled) sample = fx->process(sample);
    }
    return sample;
}