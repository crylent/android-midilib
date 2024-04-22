#ifndef FILTER_H
#define FILTER_H

#include "SoundFX.h"
#include "../AudioEngine.h"
#include "filter_includes.h"

class Filter: public SoundFX {
public:
    enum class Type { // must be the same as in Filter.kt
        AllPass, HighPass, LowPass, HighPassButterworth, LowPassButterworth, HighPassLR, LowPassLR, // simple
        HighShelf, LowShelf, // shelf
        AllPass2, HighPass2, LowPass2, BandPass, BandStop, // with Q
        BandPassButterworth, BandStopButterworth, // with bandwidth
        ParametricBoost, ParametricCut, ParametricNonConstantQ // parametric
    };

    Filter(Type type, float cornerFrequency, float q, float gain, float bandwidth);

    float process(float sample) override;

    void setType(Type type);
    void setFrequency(float frequency);
    void setQ(float Q);
    void setGain(float gain);
    void setBandwidth(float bandwidth);

private:
    static void createListener(Biquad& filter);

    template<class FilterClass> requires(is_base_of<BiquadSimple, FilterClass>() == true)
    void activateFilter();

    template<class FilterClass> requires(is_base_of<BiquadShelf, FilterClass>() == true)
    void activateFilter();

    template<class FilterClass> requires(is_base_of<BiquadQ, FilterClass>() == true)
    void activateFilter();

    template<class FilterClass> requires(is_base_of<BiquadBand, FilterClass>() == true)
    void activateFilter();

    template<class FilterClass> requires(is_base_of<BiquadParametric, FilterClass>() == true)
    void activateFilter();

    float mFrequency, mQ, mGain, mBandwidth;

    unique_ptr<Biquad> mActiveFilter;
};


#endif //FILTER_H
