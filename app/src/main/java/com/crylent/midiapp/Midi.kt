package com.crylent.midiapp

import com.crylent.midilib.AudioEngine
import com.crylent.midilib.Oscillator
import com.crylent.midilib.instrument.SynthInstrument

object Midi {
    init {
        AudioEngine.start()
    }

    val instrument = SynthInstrument()

    init {
        AudioEngine.setInstrument(0, instrument)
        instrument.addOscillator(Oscillator(Oscillator.Shape.SINE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.TRIANGLE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SAW))
        instrument.addOscillator(Oscillator(Oscillator.Shape.REVERSE_SAW))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SQUARE))
    }
}