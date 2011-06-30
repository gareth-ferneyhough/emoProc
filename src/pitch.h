#ifndef PITCH_H
#define PITCH_H

#include <aubio/aubio.h>

class Pitch
{
    public:
        Pitch(int sample_rate);
        ~Pitch();
        float getPitch(const float* const, int);

    private:
        fvec_t * yin_in;
        aubio_pitchdetection_t * o;

        int sample_rate_;
        bool is_initialized_;

        int init();
};

#endif // PITCH_H
