package com.crylent.midiapp

import com.crylent.midilib.AudioEngine
import com.crylent.midilib.Oscillator
import com.crylent.midilib.envelope.ADSREnvelope
import com.crylent.midilib.instrument.Synthesizer
import com.crylent.midilib.soundfx.FXList
import com.crylent.midilib.soundfx.Filter

object Midi {
    init {
        AudioEngine.start()
    }

    val instrument = Synthesizer(ADSREnvelope())
    val filter = Filter.simple(Filter.Type.LowPass, 1000f)

    init {
        AudioEngine.setInstrument(0, instrument)
        instrument.addOscillator(Oscillator(Oscillator.Shape.SINE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.TRIANGLE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SAW))
        instrument.addOscillator(Oscillator(Oscillator.Shape.REVERSE_SAW))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SQUARE))
        filter.disable()
        FXList.MasterFX.add(filter)
    }
}