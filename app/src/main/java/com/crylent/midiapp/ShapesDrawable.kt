package com.crylent.midiapp

import com.crylent.midilib.Oscillator

object ShapesDrawable: LinkedHashMap<Oscillator.Shape, Int>() {
    init {
        put(Oscillator.Shape.SINE, R.drawable.sine)
        put(Oscillator.Shape.TRIANGLE, R.drawable.triangle)
        put(Oscillator.Shape.SQUARE, R.drawable.square)
        put(Oscillator.Shape.SAW, R.drawable.saw)
        put(Oscillator.Shape.REVERSE_SAW, R.drawable.reverse_saw)
        put(Oscillator.Shape.CUSTOM, R.drawable.custom)
    }

    private fun readResolve(): Any = ShapesDrawable
}