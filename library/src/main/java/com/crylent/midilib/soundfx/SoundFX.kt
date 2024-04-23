package com.crylent.midilib.soundfx

abstract class SoundFX: Cloneable {
    abstract fun getId(): Int
    abstract fun getConfig(): Map<String, Number>

    fun setEnabled(enabled: Boolean) {
        if (linkedChannel != NOT_LINKED) {
            externalSetEnabled(enabled)
        }
    }
    private external fun externalSetEnabled(enabled: Boolean)
    fun enable() = setEnabled(true)
    fun disable() = setEnabled(false)

    fun getFloatParameter(param: String): Float = getConfig()[param]!!.toFloat()
    fun getIntParameter(param: String): Int = getConfig()[param]!!.toInt()

    private var linkedChannel: Byte = NOT_LINKED
    private var fxIndex: Byte = NOT_LINKED

    internal fun assignToChannel(channel: Byte) {
        linkedChannel = channel
        fxIndex = externalAssignToChannel(channel)
    }

    protected fun updateParameter(param: String, value: Float) {
        if (linkedChannel != NOT_LINKED) {
            externalEditEffect(param, value)
        }
    }
    protected fun updateIntParameter(param: String, value: Int) {
        if (linkedChannel != NOT_LINKED) {
            externalEditEffectInt(param, value)
        }
    }

    private external fun externalAssignToChannel(channel: Byte): Byte
    private external fun externalEditEffect(param: String, value: Float)
    private external fun externalEditEffectInt(param: String, value: Int)

    companion object {
        const val NOT_LINKED: Byte = -2

        const val THRESHOLD = "threshold"
        const val LIMIT = "limit"
        const val ATTACK = "attack"
        const val RELEASE = "release"
        const val TYPE = "type"
        const val FREQUENCY = "frequency"
        const val Q = "q"
        const val GAIN = "gain"
        const val BANDWIDTH = "bandwidth"
    }

    public abstract override fun clone(): SoundFX
}