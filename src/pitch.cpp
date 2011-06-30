#include "pitch.h"
#include <string.h> // for memcpy
#include <assert.h>

Pitch::Pitch(int sample_rate) :
    sample_rate_(sample_rate)
{
    //ctor
    init();
}

Pitch::~Pitch()
{
    //dtor
    del_fvec(yin_in);
    del_aubio_pitchdetection(o);
    aubio_cleanup();
}

int Pitch::init()
{
    // From aubio pitch example
    /* allocate some memory */
    uint_t win_s      = 1024;                       /* window size */
    uint_t hop_s      = win_s/4;                    /* hop size */
    uint_t samplerate = sample_rate_;               /* samplerate */
    uint_t channels   = 1;                          /* number of channel */

    aubio_pitchdetection_mode mode = aubio_pitchm_freq;
    aubio_pitchdetection_type type = aubio_pitch_yinfft;
    yin_in = new_fvec (win_s, channels); /* input buffer */
    o  = new_aubio_pitchdetection(
    win_s, hop_s, channels, samplerate, type, mode
    );

    is_initialized_ = true;

    return 0;
}

float Pitch::getPitch(const float* const audio_frames, int num_frames)
{
    if (is_initialized_ == true) {
        //assert (num_frames == 1024);

        memcpy (*(yin_in->data), audio_frames,
        sizeof (float) * num_frames);

        return aubio_pitchdetection(o,yin_in);
    }
}
