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
  del_aubio_pitch(o);
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

  //const char* pitch_mode = "freq";
  //const char* pitch_method = "yin";
  
  yin_in = new_fvec (win_s); /* input buffer */
  o  = new_aubio_pitch("yin", win_s, hop_s, samplerate);
  aubio_pitch_set_unit(o, "freq");

  is_initialized_ = true;

  return 0;
}

float Pitch::getPitch(const float* const audio_frames, int num_frames)
{
  if (is_initialized_ == true) {
    //assert (num_frames == 1024);

    memcpy (yin_in->data, audio_frames,
            sizeof (float) * num_frames);

    fvec_t *out = new_fvec(1);
    aubio_pitch_do(o, yin_in, out);

    return float(out->data[0]);
  }

  else return 0;
}
