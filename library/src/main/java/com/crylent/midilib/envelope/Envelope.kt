package com.crylent.midilib.envelope

import com.crylent.midilib.InstrumentComponent

abstract class Envelope: Cloneable, InstrumentComponent() {
    enum class Type { ADSR, ReleaseOnly }

    protected abstract fun getType(): Type
    fun getTypeId() = getType().ordinal

    public abstract override fun clone(): Envelope
}