#include "jni.h"

static FXList& getFXList(int8_t channel) {
    if (channel == -1) { // Master
        return AudioEngine::getMasterFX();
    } else {
        return AudioEngine::getChannels()[channel]->getEffects();
    }
}

static SoundFX& getEffect(JNIEnv* env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID idChannel = env->GetFieldID(cls, "linkedChannel", "B");
    jfieldID idIndex = env->GetFieldID(cls, "fxIndex", "B");
    jbyte channel = env->GetByteField(thiz, idChannel);
    jbyte i = env->GetByteField(thiz, idIndex);

    FXList& fxList = getFXList(channel);
    auto& effect = fxList.getEffect(i);
    return effect;
}