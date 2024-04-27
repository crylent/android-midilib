
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.19)

# Declares and names the project.

project("midilib")


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(
        # Sets the name of the library.
        midilib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        jni/jni_engine.cpp
        jni/jni_engine_functions.cpp
        jni/jni_instrument.cpp
        jni/jni_instrument_functions.cpp
        jni/jni_soundfx.cpp
        jni/jni_soundfx_functions.cpp
        AudioEngine.cpp
        AudioCallback.cpp
        player/SoundPlayer.cpp
        Wave.cpp
        instrument/Instrument.cpp
        instrument/Synthesizer.cpp
        instrument/Sampler.cpp
        instrument/InstrumentLib.cpp
        envelope/Envelope.cpp
        envelope/ADSREnvelope.cpp
        envelope/ReleaseOnlyEnvelope.cpp
        Channel.cpp
        NoteFrequency.cpp
        Assets.cpp
        oscillators/Oscillator.cpp
        oscillators/Detune.cpp
        oscillators/SineOscillator.cpp
        oscillators/TriangleOscillator.cpp
        oscillators/SawtoothOscillator.cpp
        oscillators/ReverseSawtoothOscillator.cpp
        oscillators/SquareOscillator.cpp
        oscillators/CustomOscillator.cpp
        soundfx/SoundFX.cpp
        soundfx/Limiter.cpp
        soundfx/Filter.cpp
        FXList.cpp
        log.cpp

        # Includes third party library
        third_party/AudioFile.h
    )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       midilib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

# Include oboe
set (OBOE_DIR ${PROJECT_SOURCE_DIR}/oboe)
add_subdirectory (${OBOE_DIR} ./oboe)
include_directories (${OBOE_DIR}/include)

# Include libsamplerate
set (LIBSR_DIR ${PROJECT_SOURCE_DIR}/libsamplerate)
add_subdirectory(${LIBSR_DIR} ./libsamplerate)
include_directories(${LIBSR_DIR}/include)

# Include DSP-Cpp-filters
set (DSP_DIR ${PROJECT_SOURCE_DIR}/DSP-Cpp-filters/lib)
add_subdirectory(${DSP_DIR} ./DSP-Cpp-filters)
include_directories(${DSP_DIR})

target_link_libraries(midilib log oboe android samplerate DSP-Cpp-filters)