package com.crylent.midilib.instrument

import com.crylent.midilib.envelope.Envelope

@Suppress("unused", "LeakingThis")
abstract class Instrument(val envelope: Envelope): Cloneable {
    /** Optional - not used in library, but can be assigned for convenience **/
    var name = ""

    init {
        envelope.owner = this
    }

    internal var libIndex: Int = NO_INDEX
        private set

    fun assignToChannel(channel: Byte) {
        if (libIndex == NO_INDEX) {
            libIndex = externalCreate()
            onCreatedListeners.forEach {
                it.onCreated(this)
            }
            onCreatedListeners.clear()
        }
        externalAssignToChannel(channel)
    }

    private val onCreatedListeners = ArrayList<OnCreatedListener>()

    fun interface OnCreatedListener {
        fun onCreated(instrument: Instrument)
    }

    fun addOnCreatedListener(listener: OnCreatedListener) {
        if (libIndex == NO_INDEX) onCreatedListeners.add(listener)
        else listener.onCreated(this) // already created
    }

    private external fun externalCreate(): Int
    private external fun externalAssignToChannel(channel: Byte)

    fun asSampler() = this as Sampler
    fun asSynthesizer() = this as Synthesizer

    public abstract override fun clone(): Instrument

    companion object {
        const val NO_INDEX = -1
    }
}