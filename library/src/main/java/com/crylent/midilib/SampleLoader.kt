package com.crylent.midilib

import android.content.Context
import android.net.Uri
import java.io.File

@Suppress("unused")
interface SampleLoader {
    fun loadSample(context: Context, sample: Sample)

    /**
     * Load asset from assets folder of application
     */
    fun loadSample(context: Context, filename: String, note: Byte = Sample.NOTE_AUTO) {
        loadSample(context, SampleFromAssets(filename, note))
    }

    /**
     * Load asset from URI using content resolver
     */
    fun loadSample(context: Context, uri: Uri, note: Byte = Sample.NOTE_AUTO) {
        loadSample(context, SampleFromUri(uri, note))
    }

    /**
     * Load asset from file
     */
    fun loadSample(context: Context, file: File, note: Byte = Sample.NOTE_AUTO) {
        loadSample(context, SampleFromFile(file, note))
    }

    /**
     * Load asset from byte array
     */
    fun loadSample(context: Context, bytes: ByteArray, note: Byte = Sample.NOTE_AUTO) {
        loadSample(context, SampleFromBytes(bytes, note))
    }
}