package com.crylent.midiapp

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.Oscillator
import kotlin.math.roundToInt

private const val OSC_INDEX = "OscillatorIndex"

/**
 * A simple [Fragment] subclass.
 * Use the [OscillatorFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class OscillatorFragment : DialogFragment() {
    private var oscIndex: Int = 0

    private lateinit var osc: Oscillator
    private lateinit var unisonVoicesPicker: NumberPicker
    private lateinit var detuneSeekbar: SeekBar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            oscIndex = it.getInt(OSC_INDEX)
            osc = Midi.instrument.getOscillator(oscIndex)
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_oscillator, container, false)
    }

    override fun onStart() {
        super.onStart()
        dialog?.apply {
            findViewById<RecyclerView>(R.id.shapes).apply {
                adapter = ShapesAdapter(requireActivity(), oscIndex)
            }
            findViewById<NumberPicker>(R.id.pitch).apply {
                value = osc.pitch
                setOnValueChangedListener { value, _ ->
                    osc.pitch = value
                }
            }
            unisonVoicesPicker = findViewById<NumberPicker>(R.id.unisonVoices).apply {
                value = osc.detune?.unisonVoices ?: 1
                setOnValueChangedListener { value, _ ->
                    if (value == 1) osc.disableDetune()
                    else if (osc.detune != null) osc.detune!!.unisonVoices = value
                    else osc.enableDetune(value, getDetuneLevel())
                }
            }
            detuneSeekbar = findViewById(R.id.detune)
            detuneSeekbar.apply {
                progress = getDetuneProgress()
                setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
                    override fun onProgressChanged(
                        seekBar: SeekBar?, progress: Int, fromUser: Boolean
                    ) {}

                    override fun onStartTrackingTouch(seekBar: SeekBar?) {}

                    override fun onStopTrackingTouch(seekBar: SeekBar?) {
                        if (unisonVoicesPicker.value == 1) return
                        if (osc.detune != null) osc.detune!!.detune = getDetuneLevel()
                        else osc.enableDetune(unisonVoicesPicker.value, getDetuneLevel())
                    }
                })
            }
        }
    }

    private fun getDetuneLevel() =
        detuneSeekbar.progress / detuneSeekbar.max.toFloat() / DETUNE_DIVISOR + Float.MIN_VALUE

    private fun getDetuneProgress(): Int {
        if (osc.detune == null) return 0
        return ((osc.detune!!.detune - Float.MIN_VALUE) * DETUNE_DIVISOR * detuneSeekbar.max).roundToInt()
    }

    companion object {
        @JvmStatic
        fun newInstance(oscIndex: Int) =
            OscillatorFragment().apply {
                arguments = Bundle().apply {
                    putInt(OSC_INDEX, oscIndex)
                }
            }

        const val TAG = "OscillatorFragment"

        private const val DETUNE_DIVISOR = 24
    }
}