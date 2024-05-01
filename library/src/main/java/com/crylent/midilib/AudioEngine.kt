package com.crylent.midilib

import com.crylent.midilib.instrument.Instrument
import java.util.LinkedList

@Suppress("MemberVisibilityCanBePrivate", "unused")
object AudioEngine {
    init {
        System.loadLibrary("midilib")
    }

    const val AUTO_DEFINITION = -1
    const val MASTER: Byte = -1

    external fun start(sharedMode: Boolean = false, sampleRate: Int = AUTO_DEFINITION, bufferSize: Int = AUTO_DEFINITION)
    external fun start()
    external fun stop()

    fun restart(sharedMode: Boolean? = null, sampleRate: Int? = null, bufferSize: Int? = null) {
        stop()
        start(
            sharedMode ?: getSharedMode(),
            sampleRate ?: getSampleRate(),
            bufferSize ?: getBufferSize()
        )
    }
    fun setSharedMode(sharedMode: Boolean) = restart(sharedMode = sharedMode)
    fun setSampleRate(sampleRate: Int) = restart(sampleRate = sampleRate)
    fun setBufferSize(bufferSize: Int) = restart(bufferSize = bufferSize)

    external fun getSharedMode(): Boolean
    external fun getSampleRate(): Int
    external fun getBufferSize(): Int

    external fun noteOn(channel: Byte, note: Byte, amplitude: Float)
    external fun noteOff(channel: Byte, note: Byte)
    external fun allNotesOff(channel: Byte)

    external fun getNumberOfChannels(): Byte

    fun setInstrument(channel: Byte, instrument: Instrument) {
        instrument.assignToChannel(channel)
    }

    data class NoteEvent(val channel: Byte, val time: Long, val note: Byte, val amplitude: Float)

    private class EventList(events: List<NoteEvent>) {
        private val _events: LinkedList<NoteEvent>

        init {
            _events = LinkedList<NoteEvent>(events.sortedBy { it.time })
        }

        private val iterator = _events.listIterator()

        fun hasNext() = iterator.hasNext()
        fun next() = iterator.next()

        fun getLength() = _events.last.time + TAIL

        companion object {
            private const val TAIL = 2000
        }
    }

    fun renderWav(events: List<NoteEvent>) = renderWavExternal(EventList(events))

    private external fun renderWavExternal(sortedEvents: EventList): ByteArray
}