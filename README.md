# Android MIDI Library (MIDI Driver)
## Installation
In settings.gradle:
```gradle
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        mavenCentral()
        maven { url 'https://jitpack.io' }
    }
}
```

In build.gradle:
```gradle
dependencies {
    implementation 'com.github.crylent:android-midilib:v2.0.1'
}
```

## Usage
```kotlin
AudioEngine.start()

// create synthesizer instrument
val synthesizer = Synthesizer(ADSREnvelope(1, 0, 1, 1)).apply {
    addOscillator(Oscillator(Oscillator.Shape.SAW))
    addOscillator(Oscillator(Oscillator.Shape.SAW, frequencyFactor = 2)) // octave up
}

// create sampler instrument
Sampler.setResamplingQuality(Sampler.ResamplingQuality.LINEAR) // very slow and can crash with better quality than linear
val sampler = Sampler(ReleaseOnlyEnvelope(1)).apply {
    loadSample(applicationContext, SampleFromAssets("pluck.wav", 60))
    copyAssetToRange(60, 0, 127)
}

// assign instruments to channels
AudioEngine.setInstrument(0, synthesizer)
AudioEngine.setInstrument(1, sampler)

// apply effects
FXList.ChannelFX[0].add(Filter(Filter.Type.LowPass, 1000f))
FXList.ChannelFX[1].add(Filter(Filter.Type.HighPass, 1000f))

// apply effects to master channel
FXList.MasterFX.add(Limiter())

// when you need to play a note
AudioEngine.noteOn(0, 60, 1.0f) // channel, note and amplitude

// when you need to stop playing a note
AudioEngine.noteOff(0, 60) // channel and note

// if you want to render WAV instead of real-time playback (playback will be paused while rendering)
val wav = AudioEngine.renderWav(listOf(
    AudioEngine.NoteEvent(0, 0, 60, 1.0f), // play C on time=0
    AudioEngine.NoteEvent(0, 1, 60, 0.0f), // stop playing C on time=1
    AudioEngine.NoteEvent(0, 1, 64, 1.0f), // play E on time=1
    AudioEngine.NoteEvent(0, 2, 64, 0.0f)  // stop playing E on time=2
))
```
