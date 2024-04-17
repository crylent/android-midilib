package com.crylent.midiapp

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.RecyclerView
import com.crylent.midilib.Oscillator
import com.lukelorusso.verticalseekbar.VerticalSeekBar

class MixerAdapter: RecyclerView.Adapter<MixerAdapter.Holder>() {
    class Holder(itemView: View): RecyclerView.ViewHolder(itemView) {
        var oscillator: Oscillator? = null
            set(value) {
                field = value
                button.setImageResource(ShapesDrawable[value?.shape]!!)
            }

        val button = itemView.findViewById<ImageButton>(R.id.shape).apply {
            setOnClickListener {
                OscillatorFragment.newInstance(adapterPosition).show(
                    (itemView.context as FragmentActivity).supportFragmentManager.beginTransaction(),
                    OscillatorFragment.TAG
                )
            }
        }
        private val volume = itemView.findViewById<VerticalSeekBar>(R.id.volume)

        init {
            volume.setOnProgressChangeListener {
                oscillator?.amplitude = it.toFloat() / volume.maxValue
            }
        }
    }

    private val holders = arrayListOf<Holder>()

    fun setShape(holderIndex: Int, shape: Oscillator.Shape) {
        holders[holderIndex].button.setImageResource(ShapesDrawable[shape]!!)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) = Holder(
        LayoutInflater.from(parent.context).inflate(R.layout.mixer_item, parent, false)
    ).apply {
        holders.add(this)
    }

    override fun getItemCount() = Midi.instrument.oscCount

    override fun onBindViewHolder(holder: Holder, position: Int) {
        holder.oscillator = Midi.instrument.getOscillator(position)
    }
}