#include "Filter.h"
#include "../log.h"

float Filter::process(float sample) {
    return mActiveFilter->process(sample);
}

/**
 * Constructs new Filter FX with provided parameters.
 * @param type type of filter
 * @param cornerFrequency a frequency for which the output of the filter is -3 dB
 * @param q quality factor (might be unused depending on type)
 * @param gain gain/attenuation in dB (might be unused depending on type)
 * @param bandwidth bandwidth of peak/notch (might be unused depending on type)
 */
Filter::Filter(Filter::Type type, float cornerFrequency, float q, float gain, float bandwidth) {
    mFrequency = cornerFrequency;
    mQ = q;
    mGain = gain;
    mBandwidth = bandwidth;
    setType(type);
}

/**
 * Changes type of the filter.
 * @param type new type
 */
void Filter::setType(Filter::Type type) {
    switch (type) {
        case Type::AllPass: activateFilter<FO_APF>(); break;
        case Type::HighPass: activateFilter<FO_HPF>(); break;
        case Type::LowPass: activateFilter<FO_LPF>(); break;
        case Type::HighPassButterworth: activateFilter<SO_BUTTERWORTH_HPF>(); break;
        case Type::LowPassButterworth: activateFilter<SO_BUTTERWORTH_LPF>(); break;
        case Type::HighPassLR: activateFilter<SO_LINKWITZ_RILEY_HPF>(); break;
        case Type::LowPassLR: activateFilter<SO_LINKWITZ_RILEY_LPF>(); break;
        case Type::HighShelf: activateFilter<FO_SHELVING_HIGH>(); break;
        case Type::LowShelf: activateFilter<FO_SHELVING_LOW>(); break;
        case Type::AllPass2: activateFilter<SO_APF>(); break;
        case Type::HighPass2: activateFilter<SO_HPF>(); break;
        case Type::LowPass2: activateFilter<SO_LPF>(); break;
        case Type::BandPass: activateFilter<SO_BPF>(); break;
        case Type::BandStop: activateFilter<SO_BSF>(); break;
        case Type::BandPassButterworth: activateFilter<SO_BUTTERWORTH_BPF>(); break;
        case Type::BandStopButterworth: activateFilter<SO_BUTTERWORTH_BSF>(); break;
        case Type::ParametricBoost: activateFilter<SO_PARAMETRIC_CQ_BOOST>(); break;
        case Type::ParametricCut: activateFilter<SO_PARAMETRIC_CQ_CUT>(); break;
        case Type::ParametricNonConstantQ: activateFilter<SO_PARAMETRIC_NCQ>(); break;
    }
    createListener(*mActiveFilter);
}

void Filter::createListener(Biquad &filter) {
    AudioEngine::AddOnStartListener([&filter]() -> void {
        filter.set_fs(AudioEngine::getSampleRate());
    });
}

/**
 * Sets new corner frequency.
 * @param cornerFrequency a frequency for which the output of the filter is -3 dB
 */
void Filter::setFrequency(float cornerFrequency) {
    mFrequency = cornerFrequency;
    mActiveFilter->set_fc(cornerFrequency);
}

/**
 * Sets new quality factor.
 * @param q quality factor (might be unused depending on type)
 */
void Filter::setQ(float q) {
    mQ = q;
    try {
        mActiveFilter->set_param(Biquad::Param::Q, q);
    } catch (invalid_argument) {
        LOGW("Parameter 'Q' is not applicable to this filter");
    }
}

/**
 * Sets new gain.
 * @param gain gain/attenuation in dB (might be unused depending on type)
 */
void Filter::setGain(float gain) {
    mGain = gain;
    try {
        mActiveFilter->set_param(Biquad::Param::GAIN, gain);
    } catch (invalid_argument) {
        LOGW("Parameter 'gain' is not applicable to this filter");
    }
}

/**
 * Sets new bandwidth.
 * @param bandwidth bandwidth of peak/notch (might be unused depending on type)
 */
void Filter::setBandwidth(float bandwidth) {
    mBandwidth = bandwidth;
    try {
        mActiveFilter->set_param(Biquad::Param::BW, bandwidth);
    } catch (invalid_argument) {
        LOGW("Parameter 'bandwidth' is not applicable to this filter");
    }
}

template<class FilterClass> requires(is_base_of<BiquadSimple, FilterClass>() == true)
void Filter::activateFilter() {
    mActiveFilter = make_unique<FilterClass>(mFrequency, AudioEngine::getSampleRate());
}

template<class FilterClass> requires(is_base_of<BiquadShelf, FilterClass>() == true)
void Filter::activateFilter() {
    mActiveFilter = make_unique<FilterClass>(mGain, mFrequency, AudioEngine::getSampleRate());
}

template<class FilterClass> requires(is_base_of<BiquadQ, FilterClass>() == true)
void Filter::activateFilter() {
    mActiveFilter = make_unique<FilterClass>(mQ, mFrequency, AudioEngine::getSampleRate());
}

template<class FilterClass> requires(is_base_of<BiquadBand, FilterClass>() == true)
void Filter::activateFilter() {
    mActiveFilter = make_unique<FilterClass>(mBandwidth, mFrequency, AudioEngine::getSampleRate());
}

template<class FilterClass> requires(is_base_of<BiquadParametric, FilterClass>() == true)
void Filter::activateFilter() {
    mActiveFilter = make_unique<FilterClass>(mGain, mQ, mFrequency, AudioEngine::getSampleRate());
}
