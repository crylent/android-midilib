#include "jni.h"
#include "jni_engine_functions.cpp"
#include "../third_party/AudioFile.h"
#include "../log.h"

#pragma ide diagnostic ignored "bugprone-reserved-identifier"

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_start__([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    AudioEngine::start();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_stop([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    AudioEngine::stop();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_start__ZII([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jboolean shared_mode,
                                               jint sample_rate, jint buffer_size) {
    if (shared_mode) {
        AudioEngine::start(oboe::SharingMode::Shared, sample_rate, buffer_size);
    } else {
        AudioEngine::start(oboe::SharingMode::Exclusive, sample_rate, buffer_size);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_noteOn([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jbyte channel, jbyte note, jfloat amplitude) {
    AudioEngine::noteOn(channel, note, amplitude);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_noteOff([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jbyte channel, jbyte note) {
    AudioEngine::noteOff(channel, note);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_crylent_midilib_AudioEngine_allNotesOff([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz, jbyte channel) {
    AudioEngine::allNotesOff(channel);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_crylent_midilib_AudioEngine_renderWavExternal(JNIEnv *env, [[maybe_unused]] jobject thiz,
                                                       jobject sorted_events) {
    AudioEngine::pause();

    jclass listCls = env->GetObjectClass(sorted_events);
    jmethodID idLength = env->GetMethodID(listCls, "getLength", "()J");
    jlong length = env->CallLongMethod(sorted_events, idLength);
    jmethodID idHasNext = env->GetMethodID(listCls, "hasNext", "()Z");
    jmethodID idNext = env->GetMethodID(listCls, "next", "()Lcom/crylent/midilib/AudioEngine$NoteEvent;");

    jclass eventCls = env->FindClass("com/crylent/midilib/AudioEngine$NoteEvent");
    jfieldID idChannel = env->GetFieldID(eventCls, "channel", "B");
    jfieldID idTime = env->GetFieldID(eventCls, "time", "J");
    jfieldID idNote = env->GetFieldID(eventCls, "note", "B");
    jfieldID idAmplitude = env->GetFieldID(eventCls, "amplitude", "F");

    auto& player = AudioEngine::getPlayer();
    auto wav = AudioFile<float>();
    wav.setSampleRate(AudioEngine::getSampleRate());
    wav.setBitDepth(32);
    size_t numSamples = length * AudioEngine::getSampleRate() / 1000;
    wav.setNumChannels(1);
    wav.samples[0].reserve(numSamples);
    size_t prevSample = 0;
    while (env->CallBooleanMethod(sorted_events, idHasNext)) {
        jobject event = env->CallObjectMethod(sorted_events, idNext);
        jbyte channel = env->GetByteField(event, idChannel);
        jlong time = env->GetLongField(event, idTime);
        size_t sample = time * AudioEngine::getSampleRate() / 1000;
        jbyte note = env->GetByteField(event, idNote);
        jfloat amplitude = env->GetFloatField(event, idAmplitude);
        renderNSamples(player, sample - prevSample, wav.samples[0]);
        prevSample = sample;
        if (amplitude != 0) {
            AudioEngine::noteOn(channel, note, amplitude);
        } else {
            AudioEngine::noteOff(channel, note);
        }
    }
    renderNSamples(player, numSamples - prevSample, wav.samples[0]);

    vector<uint8_t> wavData;
    wav.saveToMemory(wavData);

    auto size = (jsize) wavData.size();
    auto bytes = env->NewByteArray(size);
    env->SetByteArrayRegion(
            bytes,
            0, size,
            reinterpret_cast<const jbyte *>(wavData.data())
            );

    LOGI("Created WAV file, size: %d KB", size / 1024);

    AudioEngine::resume();
    return bytes;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_crylent_midilib_AudioEngine_getSharedMode([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    return AudioEngine::isShared();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_crylent_midilib_AudioEngine_getSampleRate([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    return AudioEngine::getSampleRate();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_crylent_midilib_AudioEngine_getBufferSize([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    return AudioEngine::getBufferSize();
}

extern "C"
JNIEXPORT jbyte JNICALL
Java_com_crylent_midilib_AudioEngine_getNumberOfChannels([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject thiz) {
    return AudioEngine::getNumChannels();
}