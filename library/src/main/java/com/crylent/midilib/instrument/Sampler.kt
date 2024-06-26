package com.crylent.midilib.instrument

import android.content.Context
import com.crylent.midilib.Sample
import com.crylent.midilib.SampleLoader
import com.crylent.midilib.envelope.Envelope

@Suppress("unused", "MemberVisibilityCanBePrivate")
open class Sampler(envelope: Envelope): Instrument(envelope), SampleLoader {

    private val samples = mutableListOf<Sample>()

    val samplesList get() = samples.toList()

    override fun loadSample(context: Context, sample: Sample) {
        if (sample.note < 0) throw IllegalArgumentException("Note is not specified")
        samples.add(sample)
        addOnCreatedListener {
            val bytes = sample.readBytes(context)
            externalLoadSample(bytes, bytes.size, sample.note)
        }
    }

    fun copyAssetToRange(baseNote: Byte, min: Byte, max: Byte) {
        addOnCreatedListener {
            externalCopyAssetToRange(baseNote, min, max)
        }
    }

    private external fun externalLoadSample(wavData: ByteArray, dataSize: Int, note: Byte)
    private external fun externalCopyAssetToRange(baseNote: Byte, min: Byte, max: Byte)

    override fun clone() = Sampler(envelope.clone())

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is Sampler) return false

        if (samples != other.samples) return false

        return true
    }

    override fun hashCode() = samples.hashCode()

    enum class ResamplingQuality {
        SINC_BEST_QUALITY,
        SINC_MEDIUM_QUALITY,
        SINC_FASTEST,
        ZERO_ORDER_HOLD,
        LINEAR,
    }

    companion object {
        fun setResamplingQuality(quality: ResamplingQuality) {
            externalSetResamplingQuality(quality.ordinal)
        }
        private external fun externalSetResamplingQuality(quality: Int)
    }
}