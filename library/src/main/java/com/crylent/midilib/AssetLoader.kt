package com.crylent.midilib

import android.content.Context
import android.net.Uri
import com.crylent.midilib.instrument.ApplicationAsset
import com.crylent.midilib.instrument.Asset
import com.crylent.midilib.instrument.AssetFromBytes
import com.crylent.midilib.instrument.AssetFromFile
import com.crylent.midilib.instrument.AssetFromUri
import java.io.File

interface AssetLoader {
    fun loadAsset(context: Context, asset: Asset)

    /**
     * Load asset from assets folder of application
     */
    fun loadAsset(context: Context, filename: String, note: Byte = Asset.NOTE_AUTO) {
        loadAsset(context, ApplicationAsset(filename, note))
    }

    /**
     * Load asset from URI using content resolver
     */
    fun loadAsset(context: Context, uri: Uri, note: Byte = Asset.NOTE_AUTO) {
        loadAsset(context, AssetFromUri(uri, note))
    }

    /**
     * Load asset from file
     */
    fun loadAsset(context: Context, file: File, note: Byte = Asset.NOTE_AUTO) {
        loadAsset(context, AssetFromFile(file, note))
    }

    /**
     * Load asset from byte array
     */
    fun loadAsset(context: Context, bytes: ByteArray, note: Byte = Asset.NOTE_AUTO) {
        loadAsset(context, AssetFromBytes(bytes, note))
    }
}