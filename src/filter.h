#ifndef EMO_FILTER_H
#define EMO_FILTER_H

/* Note: Requires aubio >= 0.3.3 */
#include <aubio/aubio.h>

class Filter
{
 public:
  Filter(int sample_rate);
  ~Filter();
  int doLowpassFilter(float* const, int);

 private:
  fvec_t * filter_in;
  aubio_filter_t *butter;

  int sample_rate_;
  bool is_initialized_;

  int init();
  int initializeButter();
};

#endif // EMO_FILTER_H
