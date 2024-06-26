package com.crylent.midilib

import android.content.Context
import android.util.Log
import kotlin.math.log2
import kotlin.math.pow
import kotlin.math.roundToInt

@Suppress("MemberVisibilityCanBePrivate", "unused")
class Oscillator(
    shape: Shape,
    amplitude: Number = 1f,
    phase: Number = 0f,
    frequencyFactor: Number = 1f
): Cloneable, SampleLoader, InstrumentComponent() {
    var enabled = true
        set(value) {
            field = value
            ifOwnerIsLinkedToLib {
                val synth = it.asSynthesizer()
                if (value) synth.enableOscillator(this)
                else synth.disableOscillator(this)
            }
        }

    var shape = shape
        set(value) {
            field = value
            ifOwnerIsLinkedToLib {
                externalSetShape(value.ordinal)
            }
        }

    var amplitude = amplitude.toFloat()
        set(value) {
            field = value
            ifOwnerIsLinkedToLib {
                externalSetAmplitude(value)
            }
        }

    var phase = phase.toFloat()
        set(value) {
            field = value
            ifOwnerIsLinkedToLib {
                externalSetPhase(value)
            }
        }

    var frequencyFactor = frequencyFactor.toFloat()
        set(value) {
            field = value
            usePitch = false
            ifOwnerIsLinkedToLib {
                externalSetFreqFactor(value)
            }
        }
        get() = if (!usePitch) field else 2f.pow(pitch / 12f)

    var pitch = 0
        set(value) {
            field = value
            usePitch = true
            ifOwnerIsLinkedToLib {
                externalSetFreqFactor(frequencyFactor)
            }
        }
        get() = if (usePitch) field else (12 * log2(frequencyFactor)).roundToInt()

    private var usePitch = false

    private var sample: Sample? = null
    private var context: Context? = null

    override fun loadSample(context: Context, sample: Sample) {
        if (shape != Shape.CUSTOM)
            throw UnsupportedOperationException("loadSample is available only for custom shape")
        if (sample.note != Sample.NOTE_AUTO)
            Log.w("MidiLib", "Note is unused")
        this.sample = sample
        this.context = context
        ifOwnerIsLinkedToLib {
            loadWaveform()
        }
    }


    private fun loadWaveform() {
        if (sample == null || context == null) return
        val bytes = sample!!.readBytes(context!!)
        loadWaveform(bytes, bytes.size)
    }

    private external fun loadWaveform(wavData: ByteArray, dataSize: Int)

    var detune: Detune? = null
        private set

    val oscIndex get() = owner?.asSynthesizer()?.getOscillatorIndex(this) ?: -1

    enum class Shape {
        SINE, TRIANGLE, SQUARE, SAW, REVERSE_SAW, CUSTOM
    }

    class Detune(private val owner: Oscillator, unisonVoices: Int, detune: Float) {
        var unisonVoices = unisonVoices
            set(value) {
                field = value
                owner.apply {
                    ifOwnerIsLinkedToLib {
                        externalSetUnisonVoices(value)
                    }
                }
            }

        var detune = detune
            set(value) {
                field = value
                owner.apply {
                    ifOwnerIsLinkedToLib {
                        externalSetDetuneLevel(value)
                    }
                }
            }
    }

    fun enableDetune(unisonVoices: Int, detuneLevel: Number) {
        detune = Detune(this, unisonVoices, detuneLevel.toFloat())
        ifOwnerIsLinkedToLib {
            externalEnableDetune(unisonVoices, detuneLevel.toFloat())
        }
    }
    fun disableDetune() {
        detune = null
        ifOwnerIsLinkedToLib {
            externalDisableDetune()
        }
    }

    private external fun externalSetShape(shape: Int)
    private external fun externalSetAmplitude(value: Float)
    private external fun externalSetPhase(value: Float)
    private external fun externalSetFreqFactor(value: Float)
    private external fun externalEnableDetune(unisonVoices: Int, detuneLevel: Float)
    private external fun externalDisableDetune()
    private external fun externalSetUnisonVoices(value: Int)
    private external fun externalSetDetuneLevel(value: Float)

    public override fun clone() = Oscillator(shape, amplitude, phase, frequencyFactor).also {
        if (!enabled) it.enabled = false
        if (detune != null) it.enableDetune(detune!!.unisonVoices, detune!!.detune)
    }

    companion object {
        /**
         * Linear interpolation increases custom oscillator quality, but reduces performance
         */
        external fun enableLinearInterpolation(enable: Boolean)
    }
}