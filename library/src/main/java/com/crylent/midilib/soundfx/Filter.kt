package com.crylent.midilib.soundfx

@Suppress("MemberVisibilityCanBePrivate")
class Filter(
    type: Type,
    cornerFrequency: Float,
    q: Float = 1f,
    gain: Float = 0f,
    bandwidth: Float = 50f
): SoundFX() {
    enum class Type { // must be the same as in Filter.h
        AllPass, HighPass, LowPass, HighPassButterworth, LowPassButterworth, HighPassLR, LowPassLR, // simple
        HighShelf, LowShelf, // shelf
        AllPass2, HighPass2, LowPass2, BandPass, BandStop, // with Q
        BandPassButterworth, BandStopButterworth, // with bandwidth
        ParametricBoost, ParametricCut, ParametricNonConstantQ // parametric
    }

    var type = type
        set(value) {
            field = value
            updateIntParameter(TYPE, type.ordinal)
        }
    var cornerFrequency = cornerFrequency
        set(value) {
            field = value
            updateParameter(FREQUENCY, value)
        }
    var q = q
        set(value) {
            field = value
            updateParameter(Q, value)
        }
    var gain = gain
        set(value) {
            field = value
            updateParameter(GAIN, value)
        }
    var bandwidth = bandwidth
        set(value) {
            field = value
            updateParameter(BANDWIDTH, value)
        }

    override fun getId() = 2
    override fun getConfig(): Map<String, Number> = mapOf(
        ENABLED to isEnabled,
        TYPE to type.ordinal,
        FREQUENCY to cornerFrequency,
        Q to q,
        GAIN to gain,
        BANDWIDTH to bandwidth
    )

    override fun clone() = Filter(type, cornerFrequency, q, gain, bandwidth)

    @Suppress("unused")
    companion object {
        private val simpleFilters = listOf(
            Type.AllPass, Type.HighPass, Type.LowPass,
            Type.HighPassButterworth, Type.LowPassButterworth,
            Type.HighPassLR, Type.LowPassLR
        )
        private val shelfFilters = listOf(
            Type.HighShelf, Type.LowShelf
        )
        private val qFilters = listOf(
            Type.AllPass2, Type.HighPass2, Type.LowPass2,
            Type.BandPass, Type.BandStop
        )
        private val bandFilters = listOf(
            Type.BandPassButterworth, Type.BandStopButterworth
        )
        private val parametricFilters = listOf(
            Type.ParametricBoost, Type.ParametricCut, Type.ParametricNonConstantQ
        )

        fun simple(type: Type, cornerFrequency: Float) =
            if (simpleFilters.contains(type)) Filter(type, cornerFrequency)
            else wrongType(type)
        fun q(type: Type, cornerFrequency: Float, q: Float) =
            if (qFilters.contains(type)) Filter(type, cornerFrequency, q)
            else wrongType(type)
        fun shelf(type: Type, cornerFrequency: Float, gain: Float) =
            if (shelfFilters.contains(type)) Filter(type, cornerFrequency, gain=gain)
            else wrongType(type)
        fun band(type: Type, cornerFrequency: Float, bandwidth: Float) =
            if (bandFilters.contains(type)) Filter(type, cornerFrequency, bandwidth=bandwidth)
            else wrongType(type)
        fun parametric(type: Type, cornerFrequency: Float, q: Float, gain: Float) =
            if (parametricFilters.contains(type)) Filter(type, cornerFrequency, q, gain)
            else wrongType(type)

        private fun wrongType(type: Type): Nothing =
            throw IllegalArgumentException("Filter of type '$type' should be created with other method")

        fun allPass(cornerFrequency: Float) = simple(Type.AllPass, cornerFrequency)
        fun highPass(cornerFrequency: Float) = simple(Type.HighPass, cornerFrequency)
        fun lowPass(cornerFrequency: Float) = simple(Type.LowPass, cornerFrequency)
        fun highPassButterworth(cornerFrequency: Float) = simple(Type.HighPassButterworth, cornerFrequency)
        fun lowPassButterworth(cornerFrequency: Float) = simple(Type.LowPassButterworth, cornerFrequency)
        fun highPassLR(cornerFrequency: Float) = simple(Type.HighPassLR, cornerFrequency)
        fun lowPassLR(cornerFrequency: Float) = simple(Type.LowPassLR, cornerFrequency)
        fun allPass2(cornerFrequency: Float, q: Float) = q(Type.AllPass2, cornerFrequency, q)
        fun highPass2(cornerFrequency: Float, q: Float) = q(Type.HighPass2, cornerFrequency, q)
        fun lowPass2(cornerFrequency: Float, q: Float) = q(Type.LowPass2, cornerFrequency, q)
        fun bandPass(cornerFrequency: Float, q: Float) = q(Type.BandPass, cornerFrequency, q)
        fun bandStop(cornerFrequency: Float, q: Float) = q(Type.BandStop, cornerFrequency, q)
        fun highShelf(cornerFrequency: Float, gain: Float) = shelf(Type.HighShelf, cornerFrequency, gain)
        fun lowShelf(cornerFrequency: Float, gain: Float) = shelf(Type.LowShelf, cornerFrequency, gain)
        fun bandPassButterworth(cornerFrequency: Float, bandwidth: Float) = band(Type.BandPassButterworth, cornerFrequency, bandwidth)
        fun bandStopButterworth(cornerFrequency: Float, bandwidth: Float) = band(Type.BandStopButterworth, cornerFrequency, bandwidth)
        fun parametricBoost(cornerFrequency: Float, q: Float, gain: Float) = parametric(Type.BandStop, cornerFrequency, q, gain)
        fun parametricCut(cornerFrequency: Float, q: Float, gain: Float) = parametric(Type.BandStop, cornerFrequency, q, gain)
        fun parametricNonConstantQ(cornerFrequency: Float, q: Float, gain: Float) = parametric(Type.BandStop, cornerFrequency, q, gain)
    }
}