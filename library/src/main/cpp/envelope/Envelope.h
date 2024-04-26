#ifndef ENVELOPE_H
#define ENVELOPE_H


class Envelope {
public:
    virtual float envelope(double time, double timeReleased) = 0;
    virtual ~Envelope() = default;

protected:
    static void sharpnessCheck(float sharpness);
};


#endif //ENVELOPE_H
