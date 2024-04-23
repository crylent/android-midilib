#include "jni.h"
#include "jni_soundfx_functions.cpp"
#include <map>
#include "../log.h"
#include "../soundfx/Limiter.h"
#include "../soundfx/Filter.h"

#define MASTER_CHANNEL -1

#define LIMITER 1
#define FILTER 2

#define GET_FLOAT_PARAM(env, name) auto (name) = static_cast<float>((env)->CallFloatMethod(thiz, idGetFloatParameter, JSTR((env), #name)))
#define GET_INT_PARAM(env, name) auto (name) = static_cast<int32_t>((env)->CallIntMethod(thiz, idGetIntParameter, JSTR((env), #name)))

extern "C"
JNIEXPORT jbyte JNICALL
Java_com_crylent_midilib_soundfx_SoundFX_externalAssignToChannel(JNIEnv *env, jobject thiz,
                                                                 jbyte channel) {
    jclass fxCls = env->GetObjectClass(thiz);
    jmethodID idGetId = env->GetMethodID(fxCls, "getId", "()I");
    auto fxId = static_cast<int>(env->CallIntMethod(thiz, idGetId));
    jmethodID idGetFloatParameter = env->GetMethodID(fxCls, "getFloatParameter",
                                                     "(Ljava/lang/String;)F");
    jmethodID idGetIntParameter = env->GetMethodID(fxCls, "getIntParameter",
                                                   "(Ljava/lang/String;)I");

    LOGD("FX #%d assigned to channel %d", fxId, channel);

    unique_ptr<SoundFX> effect;
    switch (fxId) {
        case LIMITER: {
            GET_FLOAT_PARAM(env, threshold);
            GET_FLOAT_PARAM(env, limit);
            GET_FLOAT_PARAM(env, attack);
            GET_FLOAT_PARAM(env, release);
            effect = make_unique<Limiter>(threshold, limit, attack, release);
            break;
        }
        case FILTER: {
            GET_INT_PARAM(env, type);
            GET_FLOAT_PARAM(env, frequency);
            GET_FLOAT_PARAM(env, q);
            GET_FLOAT_PARAM(env, gain);
            GET_FLOAT_PARAM(env, bandwidth);
            effect = make_unique<Filter>(
                    static_cast<Filter::Type>(type),
                    frequency, q, gain, bandwidth
                    );
            break;
        }
        default: // Unexpected effect ID
            throw std::runtime_error("Unexpected effect ID");
    }

    uint8_t i;
    if (channel == MASTER_CHANNEL) {
        FXList& masterFx = AudioEngine::getMasterFX();
        i = masterFx.addEffect(std::move(effect));
    } else {
        throw exception(); // Not implemented
    }
    return static_cast<jbyte>(i);
}

#undef GET_FLOAT_PARAM

#define THRESHOLD 1
#define LIMIT 2
#define ATTACK 3
#define RELEASE 4
#define FREQUENCY 5
#define Q 6
#define GAIN 7
#define BANDWIDTH 8

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_soundfx_SoundFX_externalEditEffect(JNIEnv *env, jobject thiz,
                                                            jstring param, jfloat value) {
    auto& effect = getEffect(env, thiz);
    auto _param = env->GetStringUTFChars(param, nullptr);

    map<string, int> mapping;
    mapping["threshold"] = THRESHOLD;
    mapping["limit"] = LIMIT;
    mapping["attack"] = ATTACK;
    mapping["release"] = RELEASE;
    mapping["frequency"] = FREQUENCY;
    mapping["q"] = Q;
    mapping["gain"] = GAIN;
    mapping["bandwidth"] = BANDWIDTH;

    try {
        auto& limiter = dynamic_cast<Limiter&>(effect);
        switch (mapping[_param]) {
            case THRESHOLD: limiter.setThreshold(value); break;
            case LIMIT: limiter.setLimit(value); break;
            case ATTACK: limiter.setAttack(value); break;
            case RELEASE: limiter.setRelease(value); break;
            default:
                LOGW("Parameter '%s' is not applicable to limiter", _param);
                break;
        }
        return;
    } catch (bad_cast& e) {}
    try {
        auto& filter = dynamic_cast<Filter&>(effect);
        switch (mapping[_param]) {
            case FREQUENCY: filter.setFrequency(value); break;
            case Q: filter.setQ(value); break;
            case GAIN: filter.setGain(value); break;
            case BANDWIDTH: filter.setBandwidth(value); break;
            default:
                LOGW("Parameter '%s' is not applicable to filter", _param);
                break;
        }
        return;
    } catch (bad_cast& e) {}
}

#undef THRESHOLD
#undef LIMIT
#undef ATTACK
#undef RELEASE
#undef FREQUENCY
#undef Q
#undef GAIN
#undef BANDWIDTH

#define TYPE 1

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_soundfx_SoundFX_externalEditEffectInt(JNIEnv *env, jobject thiz,
                                                               jstring param, jint value) {
    auto& effect = getEffect(env, thiz);
    auto _param = env->GetStringUTFChars(param, nullptr);

    map<string, int> mapping;
    mapping["type"] = TYPE;

    try {
        auto& filter = dynamic_cast<Filter&>(effect);
        switch (mapping[_param]) {
            case TYPE: filter.setType(static_cast<Filter::Type>(value)); break;
            default:
                LOGW("Parameter '%s' is not applicable to limiter", _param);
                break;
        }
        return;
    } catch (bad_cast& e) {}
}

#undef TYPE

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_clearEffects([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jbyte channel) {
    FXList& fxList = getFXList(channel);
    fxList.clearEffects();
}

#undef LIMITER
extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_soundfx_SoundFX_externalSetEnabled(JNIEnv *env, jobject thiz, jboolean enabled) {
    auto& effect = getEffect(env, thiz);
    effect.isEnabled = enabled;
}