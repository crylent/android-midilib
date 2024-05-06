package com.crylent.midiapp

import android.os.Bundle
import android.widget.Button
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.AudioEngine
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
    }
}