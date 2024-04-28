package com.crylent.midilib.soundfx

import com.crylent.midilib.AudioEngine
import java.util.Comparator
import java.util.function.UnaryOperator

@Suppress("unused")
class FXList private constructor(
    private val channel: Byte, val b: MutableList<SoundFX> = mutableListOf()
): MutableList<SoundFX> by b {

    override fun add(element: SoundFX): Boolean {
        element.assignToChannel(channel)
        return b.add(element)
    }

    override fun add(index: Int, element: SoundFX) {
        element.assignToChannel(channel, index)
        b.add(index, element)
    }

    override fun addAll(index: Int, elements: Collection<SoundFX>): Boolean {
        val list = elements.toList()
        for (i in elements.indices) {
            list[i].assignToChannel(channel, index + i)
        }
        return b.addAll(index, elements)
    }

    override fun addAll(elements: Collection<SoundFX>): Boolean {
        elements.forEach {
            it.assignToChannel(channel)
        }
        return b.addAll(elements)
    }

    override fun clear() {
        b.clear()
        clearEffects(channel)
    }

    override fun remove(element: SoundFX) = b.remove(element).also {
        if (it) removeEffect(channel, indexOf(element).toByte())
    }

    override fun removeAll(elements: Collection<SoundFX>): Boolean {
        var modified = false
        elements.forEach {
             modified = modified || remove(it)
        }
        return modified
    }

    override fun removeAt(index: Int) = b.removeAt(index).also {
        removeEffect(channel, index.toByte())
    }

    override fun replaceAll(operator: UnaryOperator<SoundFX>) = throw UnsupportedOperationException()
    override fun retainAll(elements: Collection<SoundFX>) = throw UnsupportedOperationException()

    override fun set(index: Int, element: SoundFX) = removeAt(index).also {
        add(index, element)
    }

    override fun sort(c: Comparator<in SoundFX>?) = throw UnsupportedOperationException()

    fun move(element: SoundFX, newIndex: Int) = move(indexOf(element), newIndex)

    fun move(index: Int, newIndex: Int): Boolean {
        if (index >= size || newIndex >= size) return false
        b.add(newIndex, b.removeAt(index))
        moveEffect(channel, index.toByte(), newIndex.toByte())
        return true
    }

    private external fun moveEffect(channel: Byte, index: Byte, newIndex: Byte)
    private external fun clearEffects(channel: Byte)
    private external fun removeEffect(channel: Byte, index: Byte)

    companion object {
        val MasterFX = FXList(AudioEngine.MASTER)
        var ChannelFX = List(AudioEngine.getNumberOfChannels().toInt()) { FXList(it.toByte()) }
    }
}