#include "jni.h"
#include "jni_instrument_functions.cpp"
#include "../instrument/InstrumentLib.h"
#include "../instrument/Synthesizer.h"
#include "../instrument/Sampler.h"
#include "../oscillators/SineOscillator.h"
#include "../oscillators/TriangleOscillator.h"
#include "../oscillators/SquareOscillator.h"
#include "../oscillators/SawtoothOscillator.h"
#include "../oscillators/ReverseSawtoothOscillator.h"
#include "../Assets.h"
#include "../log.h"

#define GET_SYNTH(index) (dynamic_cast<Synthesizer&>(*InstrumentLib::getInstrument(index)))

#define INSTRUMENT_SYNTHESIZER 0
#define INSTRUMENT_SAMPLER 1

extern "C"
JNIEXPORT jint JNICALL
Java_com_crylent_midilib_instrument_Instrument_externalCreate(JNIEnv *env, jobject thiz) {
    jclass instCls = env->GetObjectClass(thiz);
    jclass synthInstCls = env->FindClass("com/crylent/midilib/instrument/Synthesizer");
    uint8_t instType;
    if (env->IsInstanceOf(thiz, synthInstCls)) instType = INSTRUMENT_SYNTHESIZER;
    else instType = INSTRUMENT_SAMPLER;

#define GET_FIELD(field) jfieldID id_##field = env->GetFieldID(instCls, #field, "F"); \
jfloat field = env->GetFloatField(thiz, id_##field)

    GET_FIELD(attack);
    GET_FIELD(decay);
    GET_FIELD(sustain);
    GET_FIELD(release);
    GET_FIELD(attackSharpness);
    GET_FIELD(decaySharpness);
    GET_FIELD(releaseSharpness);

#undef GET_FIELD

    uint32_t position;

    if (instType == INSTRUMENT_SYNTHESIZER) {
        jmethodID idAsSynthInst = env->GetMethodID(instCls, "asSynthesizer",
                                                   "()Lcom/crylent/midilib/instrument/Synthesizer;");
        thiz = env->CallObjectMethod(thiz, idAsSynthInst);
        auto inst = make_shared<Synthesizer>(
                attack, decay, sustain, release, attackSharpness, decaySharpness, releaseSharpness);
        jmethodID idOscCount = env->GetMethodID(synthInstCls, "getOscCount", "()I");
        jint oscillatorsCount = env->CallIntMethod(thiz, idOscCount);

        jmethodID idGetOscillator = env->GetMethodID(synthInstCls, "getOscillator",
                                                     "(I)Lcom/crylent/midilib/Oscillator;");

        for (int i = 0; i < oscillatorsCount; i++) {
            jobject oscillator = env->CallObjectMethod(thiz, idGetOscillator, i);
            addOscillator(env, *inst, oscillator);
        }
        position = InstrumentLib::addInstrument(inst);
    } else {
        auto inst = make_shared<Sampler>(
                attack, decay, sustain, release, attackSharpness, decaySharpness, releaseSharpness);
        position = InstrumentLib::addInstrument(inst);
    }
    return (jint) position;
}

#define NO_INDEX (-1)

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Synthesizer_externalAddOscillator(JNIEnv *env, jobject thiz,
                                                                      jobject oscillator) {
    int32_t index = getLibIndex(env, thiz);
    auto& inst = GET_SYNTH(index);
    addOscillator(env, inst, oscillator);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Synthesizer_externalRemoveOscillator(JNIEnv *env,
                                                                         jobject thiz,
                                                                         jint index) {
    int32_t instIndex = getLibIndex(env, thiz);
    auto& inst = GET_SYNTH(instIndex);
    inst.removeOscillator(index);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Synthesizer_externalEnableOscillator(JNIEnv *env, jobject thiz,
                                                                         jint index) {
    int32_t instIndex = getLibIndex(env, thiz);
    auto& inst = GET_SYNTH(instIndex);
    inst.enableOscillator(index);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Synthesizer_externalDisableOscillator(JNIEnv *env, jobject thiz,
                                                                          jint index) {
    int32_t instIndex = getLibIndex(env, thiz);
    auto& inst = GET_SYNTH(instIndex);
    inst.disableOscillator(index);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Instrument_externalAssignToChannel(JNIEnv *env, jobject thiz,
                                                                       jbyte channel) {
    AudioEngine::getChannels()[channel]->setInstrument(
            InstrumentLib::getInstrument(getLibIndex(env, thiz))
            );
}

#define PARAM_SETTER(param) extern "C" JNIEXPORT void JNICALL \
Java_com_crylent_midilib_instrument_Instrument_externalSet##param \
(JNIEnv *env, jobject thiz, jfloat value) {\
    int32_t index = getLibIndex(env, thiz);\
    if (index != NO_INDEX) {\
        InstrumentLib::getInstrument(index)->set##param(value);\
    }\
}

PARAM_SETTER(Attack)
PARAM_SETTER(Decay)
PARAM_SETTER(Sustain)
PARAM_SETTER(Release)
PARAM_SETTER(AttackSharpness)
PARAM_SETTER(DecaySharpness)
PARAM_SETTER(ReleaseSharpness)

#undef PARAM_SETTER

#define OSC_FUNCTION_BEGIN(env, obj) \
int32_t instIndex = getOwnerLibIndex(env, obj); \
int32_t oscIndex = getOscIndex(env, obj); \
auto& inst = GET_SYNTH(instIndex)

#define OSC_PARAM_SETTER(param) extern "C" JNIEXPORT void JNICALL \
Java_com_crylent_midilib_Oscillator_externalSet##param \
(JNIEnv *env, jobject thiz, jfloat value) { \
    OSC_FUNCTION_BEGIN(env, thiz); \
    if (instIndex != NO_INDEX && oscIndex != NO_INDEX) inst.getOscillatorByIndex(oscIndex).set##param(value); \
}

OSC_PARAM_SETTER(Amplitude)
OSC_PARAM_SETTER(Phase)
OSC_PARAM_SETTER(FreqFactor)

#undef OSC_PARAM_SETTER

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_externalSetShape(JNIEnv *env, jobject thiz, jint shape) {
    OSC_FUNCTION_BEGIN(env, thiz);
    switch (shape) {
        case SHAPE_SINE:
            inst.setOscillatorShape<SineOscillator>(oscIndex); break;
        case SHAPE_TRIANGLE:
            inst.setOscillatorShape<TriangleOscillator>(oscIndex); break;
        case SHAPE_SQUARE:
            inst.setOscillatorShape<SquareOscillator>(oscIndex); break;
        case SHAPE_SAW:
            inst.setOscillatorShape<SawtoothOscillator>(oscIndex); break;
        case SHAPE_REVERSE_SAW:
            inst.setOscillatorShape<ReverseSawtoothOscillator>(oscIndex); break;
        case SHAPE_CUSTOM:
            inst.setOscillatorShape<CustomOscillator>(oscIndex); break;
        default:
            break;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_loadWaveform(JNIEnv *env, jobject thiz, jbyteArray wav_data,
                                                 jint data_size) {
    OSC_FUNCTION_BEGIN(env, thiz);
    auto& osc = dynamic_cast<CustomOscillator&>(inst.getOscillatorByIndex(oscIndex));
    auto array = env->GetByteArrayElements(wav_data, nullptr);
    auto data = vector<uint8_t>(array, array + data_size);
    osc.loadWaveform(data);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_externalEnableDetune(JNIEnv *env, jobject thiz,
                                                         jint unison_voices, jfloat detune_level) {
    OSC_FUNCTION_BEGIN(env, thiz);
    inst.getOscillatorByIndex(oscIndex).setDetune(unison_voices, detune_level);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_externalDisableDetune(JNIEnv *env, jobject thiz) {
    OSC_FUNCTION_BEGIN(env, thiz);
    inst.getOscillatorByIndex(oscIndex).clearDetune();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_externalSetUnisonVoices(JNIEnv *env, jobject thiz,
                                                            jint value) {
    OSC_FUNCTION_BEGIN(env, thiz);
    inst.getOscillatorByIndex(oscIndex).getDetune().setUnisonVoices(value);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_Oscillator_externalSetDetuneLevel(JNIEnv *env, jobject thiz,
                                                           jfloat value) {
    OSC_FUNCTION_BEGIN(env, thiz);
    inst.getOscillatorByIndex(oscIndex).getDetune().setDetune(value);
}

#undef OSC_FUNCTION_BEGIN
#undef GET_SYNTH
#undef INSTRUMENT_SYNTHESIZER

#define GET_SAMPLER(index) (dynamic_cast<Sampler&>(*InstrumentLib::getInstrument(index)))

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Sampler_externalLoadAsset(JNIEnv *env, jobject thiz,
                                                              jbyteArray wav_data,
                                                              jint data_size, jbyte note) {
    int32_t index = getLibIndex(env, thiz);
    auto array = env->GetByteArrayElements(wav_data, nullptr);
    auto data = vector<uint8_t>(array, array + data_size);
    auto& inst = GET_SAMPLER(index);
    inst.loadAsset(note, data);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Sampler_copyAssetToRange(JNIEnv *env, jobject thiz,
                                                             jbyte baseNote,
                                                             jbyte min, jbyte max) {
    int32_t index = getLibIndex(env, thiz);
    auto& inst = GET_SAMPLER(index);
    inst.copySampleToRange(baseNote, min, max);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_instrument_Sampler_00024Companion_externalSetResamplingQuality(
        [[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jint quality) {
    Assets::setResamplingQuality(quality);
}

#undef GET_SAMPLER
#undef INSTRUMENT_SAMPLER
#undef NO_INDEX