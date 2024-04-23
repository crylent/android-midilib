package com.crylent.midilib.instrument

import android.content.Context
import com.crylent.midilib.AssetLoader

@Suppress("unused", "MemberVisibilityCanBePrivate")
open class AssetInstrument: Instrument(0, 0, 1, 0), AssetLoader {

    private val assets = mutableListOf<Asset>()

    val assetsList get() = assets.toList()

    override fun loadAsset(context: Context, asset: Asset) {
        if (asset.note < 0) throw IllegalArgumentException("Note is not specified")
        assets.add(asset)
        addOnCreatedListener {
            val bytes = asset.readBytes(context)
            externalLoadAsset(bytes, bytes.size, asset.note)
        }
    }

    private external fun externalLoadAsset(wavData: ByteArray, dataSize: Int, note: Byte)
    external fun copyAssetToRange(baseNote: Byte, min: Byte, max: Byte)

    override fun clone() = AssetInstrument()

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is AssetInstrument) return false

        if (assets != other.assets) return false

        return true
    }

    override fun hashCode() = assets.hashCode()

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