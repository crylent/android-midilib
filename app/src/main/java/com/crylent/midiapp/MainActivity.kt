package com.crylent.midiapp

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.AdapterView
import android.widget.AdapterView.OnItemSelectedListener
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.SeekBar
import android.widget.Spinner
import androidx.activity.enableEdgeToEdge
import androidx.annotation.IdRes
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.AudioEngine
import com.crylent.midilib.envelope.ADSREnvelope
import com.crylent.midilib.soundfx.Filter
import com.google.android.material.switchmaterial.SwitchMaterial
import kotlin.math.pow
import kotlin.math.roundToInt

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Midi.start(applicationContext)

        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        findViewById<RecyclerView>(R.id.mixer).apply {
            adapter = MixerAdapter()
        }

        findViewById<Button>(R.id.playNote).apply {
            setOnClickListener {
                AudioEngine.noteOn(0, 60, 1f)
            }
        }
        findViewById<Button>(R.id.playChord).apply {
            setOnClickListener {
                AudioEngine.noteOn(0, 60, 0.25f)
                AudioEngine.noteOn(0, 63, 0.25f)
                AudioEngine.noteOn(0, 67, 0.25f)
                AudioEngine.noteOn(0, 70, 0.25f)
            }
        }
        findViewById<Button>(R.id.stop).apply {
            setOnClickListener {
                AudioEngine.allNotesOff(0)
            }
        }

        val envelope = Midi.instrument.envelope as ADSREnvelope
        val attack = paramToProgress(envelope.attack)
        val decay = paramToProgress(envelope.decay)
        Log.d("midilib", decay.toString())
        val release = paramToProgress(envelope.release)
        createEnvelopeParam(R.id.attack, attack) { envelope.attack = progressToParam(it) }
        createEnvelopeParam(R.id.decay, decay) { envelope.decay = progressToParam(it) }
        createEnvelopeParam(R.id.sustain, envelope.sustain) { envelope.sustain = it }
        createEnvelopeParam(R.id.release, release) { envelope.release = progressToParam(it) }

        findViewById<SwitchMaterial>(R.id.filterSwitch).apply {
            setOnCheckedChangeListener { _, checked ->
                Midi.filter.enabled = checked
            }
        }

        val filters = mapOf(
            "Low Pass" to Filter.Type.LowPass,
            "High Pass" to Filter.Type.HighPass,
            "Band Pass" to Filter.Type.BandPass,
            "Band Stop" to Filter.Type.BandStop
        )
        findViewById<Spinner>(R.id.filterSpinner).apply {
            adapter = ArrayAdapter(
                this@MainActivity,
                android.R.layout.simple_spinner_dropdown_item,
                filters.keys.toList()
            )
            onItemSelectedListener = object : OnItemSelectedListener {
                override fun onItemSelected(
                    parent: AdapterView<*>?,
                    view: View?,
                    position: Int,
                    id: Long
                ) {
                    Midi.filter.type = filters.values.toList()[position]
                }

                override fun onNothingSelected(parent: AdapterView<*>?) {}
            }
        }

        findViewById<SwitchMaterial>(R.id.limiterSwitch).apply {
            setOnCheckedChangeListener { _, checked ->
                Midi.limiter.enabled = checked
            }
        }

        findViewById<SeekBar>(R.id.limiterGain).apply {
            progress = (max - Midi.limiter.threshold * max).roundToInt()
            setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    Midi.limiter.threshold =
                        if (progress == max) 0.001f
                        else (max - progress) / max.toFloat()
                }

                override fun onStartTrackingTouch(seekBar: SeekBar?) {}

                override fun onStopTrackingTouch(seekBar: SeekBar?) {}
            })
        }
    }

    private fun paramToProgress(value: Float) = (value / MULTIPLIER).pow(1f / POW)
    private fun progressToParam(progress: Float) = MULTIPLIER * progress.pow(POW)

    private fun createEnvelopeParam(@IdRes id: Int, value: Float, lambda: (value: Float)->Unit) {
        findViewById<SeekBar>(id).apply {
            progress = (value * max).roundToInt()
            setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {}

                override fun onStartTrackingTouch(seekBar: SeekBar?) {}

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                    lambda(progress.toFloat() / max)
                }
            })
        }
    }

    companion object {
        private const val POW = 3f
        private const val MULTIPLIER = 10f
    }
}