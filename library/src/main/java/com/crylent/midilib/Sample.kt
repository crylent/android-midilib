package com.crylent.midilib

import android.content.Context
import android.net.Uri
import java.io.File
import java.io.InputStream

sealed interface Sample {
    val note: Byte

    fun getInputStream(context: Context): InputStream

    fun readBytes(context: Context): ByteArray {
        getInputStream(context).apply {
            val bytes = readBytes()
            close()
            return bytes
        }
    }

    companion object {
        const val NOTE_AUTO: Byte = -1
    }
}

data class SampleFromAssets(val filename: String, override val note: Byte = Sample.NOTE_AUTO): Sample {
    override fun getInputStream(context: Context): InputStream = context.assets.open(filename)
}

data class SampleFromUri(val uri: Uri, override val note: Byte = Sample.NOTE_AUTO): Sample {
    override fun getInputStream(context: Context): InputStream = context.contentResolver.openInputStream(uri)!!
}

data class SampleFromFile(val file: File, override val note: Byte = Sample.NOTE_AUTO): Sample {
    override fun getInputStream(context: Context): InputStream = file.inputStream()
}

data class SampleFromBytes(val bytes: ByteArray, override val note: Byte = Sample.NOTE_AUTO): Sample {
    override fun getInputStream(context: Context) = bytes.inputStream()
    override fun readBytes(context: Context) = bytes

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is SampleFromBytes) return false
        return bytes.contentEquals(other.bytes)
    }

    override fun hashCode() = bytes.contentHashCode()
}