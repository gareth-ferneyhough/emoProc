#include "filter.h"
#include <string.h> // for memcpy
#include <assert.h>

Filter::Filter(int sample_rate) :
  sample_rate_(sample_rate)
{
  //ctor
  init();
}

Filter::~Filter()
{
  //dtor

  /* cannot delete this without segfault! crap. */
  //del_fvec(filter_in); 
  del_aubio_filter(butter);
  aubio_cleanup();
}

int Filter::init()
{
  filter_in = new_fvec(0);

  initializeButter();
  is_initialized_ = true;

  return 0;
}

int Filter::initializeButter()
{
  /* create lowpass butterworth filter.
     these params are for sample rate 44100 */

  assert(sample_rate_ == 44100);

  butter = new_aubio_filter(7);

  lvec_t *a = aubio_filter_get_feedback(butter);
  lvec_t *b = aubio_filter_get_feedforward(butter);

  a->data[0] = 1.00000000000000000000;
  a->data[1] = -5.77981206364905908401;
  a->data[2] = 13.92317084650361991294;
  a->data[3] = -17.89289935564788791567;
  a->data[4] = 12.93786955236342706144;
  a->data[5] = -4.99066192992271240314;
  a->data[6] = 0.80233298109164030709;

  b->data[0] = 0.00000000048029755494;
  b->data[1] = 0.00000000288178532962;
  b->data[2] = 0.00000000720446332405;
  b->data[3] = 0.00000000960595109873;
  b->data[4] = 0.00000000720446332405;
  b->data[5] = 0.00000000288178532962;
  b->data[6] = 0.00000000048029755494;

  a = NULL;
  b = NULL;

  return 0;
}

/* do inplace lowpass filter */
int Filter::doLowpassFilter(float* const audio_frames, int num_frames)
{
  /* do butterworth lowpass */
  filter_in->length = num_frames;

  /* todo: make this safer */
  filter_in->data = audio_frames;

  aubio_filter_do(butter, filter_in);

  return 0;
}
