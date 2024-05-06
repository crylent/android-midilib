package com.crylent.midiapp

import android.content.Context
import com.crylent.midilib.AudioEngine
import com.crylent.midilib.Oscillator
import com.crylent.midilib.envelope.ADSREnvelope
import com.crylent.midilib.instrument.Synthesizer
import com.crylent.midilib.soundfx.FXList
import com.crylent.midilib.soundfx.Filter
import com.crylent.midilib.soundfx.Limiter

object Midi {
    init {
        AudioEngine.start()
    }

    val instrument = Synthesizer(ADSREnvelope())
    val filter = Filter.simple(Filter.Type.LowPass, 1000f).apply { disable() }
    val limiter = Limiter().apply { disable() }

    fun start(context: Context) {
        AudioEngine.setInstrument(0, instrument)
        instrument.addOscillator(Oscillator(Oscillator.Shape.SINE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.TRIANGLE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SAW))
        instrument.addOscillator(Oscillator(Oscillator.Shape.SQUARE))
        instrument.addOscillator(Oscillator(Oscillator.Shape.CUSTOM).apply {
            loadSample(context, ORGAN_SAMPLE)
        })
        FXList.MasterFX.apply {
            add(filter)
            add(limiter)
        }
    }

    const val ORGAN_SAMPLE = "samples/AKWF_eorgan_0030.wav"
}