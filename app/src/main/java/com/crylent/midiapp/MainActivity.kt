package com.crylent.midiapp

import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import android.widget.AdapterView.OnItemSelectedListener
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.AudioEngine
import com.crylent.midilib.soundfx.Filter
import com.google.android.material.switchmaterial.SwitchMaterial

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

        findViewById<Button>(R.id.play).apply {
            setOnClickListener {
                AudioEngine.noteOn(0, 60, 1f)
            }
        }

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
    }
}