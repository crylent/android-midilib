#include "Envelope.h"
#include <stdexcept>

void Envelope::sharpnessCheck(float sharpness) {
    if (sharpness <= 0) {
        throw std::invalid_argument("Sharpness value must be positive");
    }
}
