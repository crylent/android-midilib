package com.crylent.midilib.soundfx

abstract class SoundFX: Cloneable {
    var enabled = true
        set(value) {
            field = value
            if (linkedChannel != NOT_LINKED) {
                externalSetEnabled(enabled)
            }
        }

    protected val isEnabled get() = if (enabled) 1 else 0

    abstract fun getId(): Int
    abstract fun getConfig(): Map<String, Number>

    private external fun externalSetEnabled(enabled: Boolean)
    fun enable() { enabled = true }
    fun disable() { enabled = false }

    fun getFloatParameter(param: String): Float = getConfig()[param]!!.toFloat()
    fun getIntParameter(param: String): Int = getConfig()[param]!!.toInt()

    private var linkedChannel: Byte = NOT_LINKED
    private var fxIndex: Byte = NOT_LINKED

    internal fun assignToChannel(channel: Byte, index: Byte = PUSH_BACK) {
        linkedChannel = channel
        fxIndex = externalAssignToChannel(channel, index)
    }

    internal fun assignToChannel(channel: Byte, index: Int) = assignToChannel(channel, index.toByte())

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

    private external fun externalAssignToChannel(channel: Byte, index: Byte): Byte
    private external fun externalEditEffect(param: String, value: Float)
    private external fun externalEditEffectInt(param: String, value: Int)

    companion object {
        const val NOT_LINKED: Byte = -2

        const val PUSH_BACK: Byte = -1

        const val ENABLED = "enabled"
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