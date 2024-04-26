package com.crylent.midilib

import com.crylent.midilib.instrument.Instrument

abstract class InstrumentComponent {
    internal var owner: Instrument? = null
        set(value) {
            if (owner != null) throw RuntimeException("This component has another owner already")
            field = value
        }
    private val ownerLibIndex get() = owner?.libIndex ?: Instrument.NO_INDEX

    protected fun ifOwnerIsLinkedToLib(lambda: (owner: Instrument)->Unit) {
        if (ownerLibIndex != Instrument.NO_INDEX) lambda(owner!!)
    }
}