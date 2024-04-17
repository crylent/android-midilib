package com.crylent.midiapp

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.RecyclerView

private const val OSC_INDEX = "OscillatorIndex"

/**
 * A simple [Fragment] subclass.
 * Use the [OscillatorFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class OscillatorFragment : DialogFragment() {
    private var oscIndex: Int = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            oscIndex = it.getInt(OSC_INDEX)
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
                val osc = Midi.instrument.getOscillator(oscIndex)
                value = osc.pitch
                setOnValueChangedListener { value, _ ->
                    osc.pitch = value
                }
            }
        }
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
    }
}