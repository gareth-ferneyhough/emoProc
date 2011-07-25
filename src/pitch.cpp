#include "pitch.h"
#include <string.h> // for memcpy
#include <assert.h>

#include <iostream>
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
  del_fvec(yin_out);
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

  yin_in = new_fvec (win_s); /* input buffer */
  yin_out = new_fvec(1);
  o  = new_aubio_pitch("yin", win_s, hop_s, samplerate);
  aubio_pitch_set_unit(o, "freq");

  is_initialized_ = true;

  return 0;
}

float Pitch::getPitch(const float* const audio_frames, int num_frames)
{
  if (is_initialized_ == true && num_frames == 1024) {
    /* Note: when disconnecting, num_frames jumps tp 2048.
       Make sure no memory is getting trashed elsewhere */

    yin_in->length = num_frames;

    /* todo: no need to copy data */
    memcpy (yin_in->data, audio_frames,
            sizeof (float) * num_frames);

    aubio_pitch_do(o, yin_in, yin_out);

    return float(yin_out->data[0]);
  }

  else{ std::cout << "shit\n"; return 0;}
}
