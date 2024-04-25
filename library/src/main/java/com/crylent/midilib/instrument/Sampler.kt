package com.crylent.midilib.instrument

import android.content.Context
import com.crylent.midilib.Sample
import com.crylent.midilib.SampleLoader

@Suppress("unused", "MemberVisibilityCanBePrivate")
open class Sampler: Instrument(0, 0, 1, 0), SampleLoader {

    private val samples = mutableListOf<Sample>()

    val samplesList get() = samples.toList()

    override fun loadSample(context: Context, sample: Sample) {
        if (sample.note < 0) throw IllegalArgumentException("Note is not specified")
        samples.add(sample)
        addOnCreatedListener {
            val bytes = sample.readBytes(context)
            externalLoadAsset(bytes, bytes.size, sample.note)
        }
    }

    private external fun externalLoadAsset(wavData: ByteArray, dataSize: Int, note: Byte)
    external fun copyAssetToRange(baseNote: Byte, min: Byte, max: Byte)

    override fun clone() = Sampler()

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