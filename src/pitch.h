#ifndef EMO_PITCH_H
#define EMO_PITCH_H

/* Note: Requires aubio >= 0.3.3 */
#include <aubio/aubio.h>

class Pitch
{
 public:
  Pitch(int sample_rate);
  ~Pitch();
  float getPitch(const float* const, int);

 private:
  fvec_t * yin_in;
  fvec_t * yin_out;
  aubio_pitch_t * o;
  aubio_filter_t *butter;

  int sample_rate_;
  bool is_initialized_;

  int init();
  int initializeButter();
};

#endif // EMO_PITCH_H
