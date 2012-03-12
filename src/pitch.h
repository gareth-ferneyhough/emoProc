#ifndef EMO_PITCH_H
#define EMO_PITCH_H

#include <vector>
#include "getf0.h"

class Pitch
{
 public:
  Pitch( int sample_rate, int& input_buffer_size, int& stride);
  ~Pitch();
  int getPitch(double* input_frames, int num_frames);

 private:
  int init(int&, int&);

  int sample_rate;
  F0_params* parameters;
  int input_buffer_size;
  int stride;
  double* input_frames;
  std::vector<double> input;
};

#endif // EMO_PITCH_H
