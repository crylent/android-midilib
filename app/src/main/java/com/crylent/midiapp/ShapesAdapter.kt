package com.crylent.midiapp

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.Oscillator

class ShapesAdapter(
    private val mainActivity: FragmentActivity,
    private val oscIndex: Int
): RecyclerView.Adapter<ShapesAdapter.Holder>() {
    val oscillator = Midi.instrument.getOscillator(oscIndex)

    class Holder(itemView: View, adapter: ShapesAdapter): RecyclerView.ViewHolder(itemView) {
        var shape: Oscillator.Shape? = null

        val button = itemView as ImageButton

        init {
            val mixerAdapter = adapter.mainActivity.findViewById<RecyclerView>(R.id.mixer).adapter
                    as MixerAdapter
            button.setOnClickListener {
                adapter.holders.forEach { holder ->
                    holder.button.isSelected = false
                }
                it.isSelected = true
                adapter.oscillator.shape = shape!!
                if (shape == Oscillator.Shape.CUSTOM) {
                    adapter.oscillator.loadSample(adapter.mainActivity, Midi.ORGAN_SAMPLE)
                }

                mixerAdapter.setShape(adapter.oscIndex, shape!!)
            }
        }
    }

    private val holders = arrayListOf<Holder>()

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) = Holder(
        LayoutInflater.from(parent.context).inflate(R.layout.shape, parent, false),
        this
    ).apply {
        holders.add(this)
    }


    override fun onBindViewHolder(holder: Holder, position: Int) {
        holder.apply {
            shape = Oscillator.Shape.entries[position]
            button.setImageResource(ShapesDrawable[shape]!!)
            button.isSelected = (oscillator.shape == shape)
        }
    }

    override fun getItemCount() = Oscillator.Shape.entries.count()
}
