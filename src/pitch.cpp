#include <iostream>
#include <algorithm>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcpy
#include <assert.h>

#include "pitch.h"

Pitch::Pitch(int new_sample_rate, int& new_input_buffer_size, int& new_stride) :
  sample_rate(new_sample_rate)
{
  //ctor
  init(new_input_buffer_size, new_stride);
}

Pitch::~Pitch()
{
  //dtor
}

int Pitch::init(int& new_input_buffer_size, int& new_stride)
{
  // Set parameters
  parameters = (F0_params *) malloc(sizeof(F0_params));
  parameters->cand_thresh = 0.3;
  parameters->lag_weight = 0.3;
  parameters->freq_weight = 0.02;
  parameters->trans_cost = 0.005;
  parameters->trans_amp = 0.5;
  parameters->trans_spec = 0.5;
  parameters->voice_bias = 0.5;//0.0;
  parameters->double_cost = 0.35;
  parameters->min_f0 = 50;
  parameters->max_f0 = 550;
  parameters->frame_step = 0.01;//0.01;
  parameters->wind_dur = 0.0075;
  parameters->n_cands = 20;
  parameters->mean_f0 = 200;         /* unused */
  parameters->mean_f0_weight = 0.0;  /* unused */
  parameters->conditioning = 0;      /* unused */

  // Check parameters
  if(check_f0_params(parameters, sample_rate)){
    fprintf(stderr, "invalid/inconsistent parameters -- exiting.\n");
    exit(1);
  }

  /* Initialize variables in get_f0.c; allocate data structures;
   * determine length and overlap of input frames to read.
   */
  if (init_dp_f0(sample_rate, parameters, &input_buffer_size, &stride)
      || input_buffer_size > INT_MAX || stride > INT_MAX)
    {
      fprintf(stderr, "problem in init_dp_f0().\n");
      exit(1);
    }

  fprintf(stderr, "init_dp_f0 returned buff_size %ld, sdstep %ld.\n",
          input_buffer_size, stride);


  input_frames = new double[input_buffer_size];

  new_input_buffer_size = input_buffer_size;
  new_stride = stride;

  return 0;
}

int Pitch::getPitch(double* new_input_frames, int num_frames)
{
  // ensure that we have enough frames in deque
  int frames_available = num_frames;
  if (frames_available < input_buffer_size)
    return 0;

  // else copy to contiguous memory
  //std::copy( new_input_frames, new_input_frames + num_frames, input_frames);
  for(int i = 0; i < num_frames; ++i)
    input_frames[i] = new_input_frames[i];

  assert(input_buffer_size == num_frames);

  //test
  double sum = 0;
  for (int i = 0; i < input_buffer_size; ++i){
    sum += input_frames[i];
  }    

  std::cout << "sum: " << sum << std::endl;

  // get pitch
  float *pitch, *probability_voicing, *rms_speech, *acpkp;
  int num_samples_returned = 0;
  int done = 0;

  // Dont know why input frames has to be cast to float, but doesnt work otherwise :/
  if (dp_f0((float*)&input_frames[0], input_buffer_size, stride, sample_rate, parameters,
            &pitch, &probability_voicing, &rms_speech, &acpkp, &num_samples_returned, done)) {
    fprintf(stderr, "problem in dp_f0().\n");
    exit(1);
  }

  // Save stats for region
  for (int i = num_samples_returned - 1; i >= 0; i--) {
    printf("%f",    pitch[i]);
    printf(",%f",   probability_voicing[i]);
    printf(",%f",   rms_speech[i]);
    printf(",%f\n", acpkp[i]);
  }

  return 1;
}
