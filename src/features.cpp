#include <assert.h>
#include <fstream>
#include <iostream>

#include "stdafx.h"
#include "interpolation.h"

#include "features.h"

Features::Features() :
  current_features_(NULL),
  count_(0)
{
  reset();
}

Features::~Features()
{
  if(current_features_ != NULL)
    delete current_features_;

  writeFeatures();
}

void Features::savePitch(float pitch)
{
  if (pitch > 0)
    pitches_.push_back(pitch);
}

void Features::saveRaw(const float* const audio_frames, int num_frames)
{
  // int current_size = raw_audio_data_.size();
  // raw_audio_data_.resize(current_size + num_frames);
  // copy(audio_frames, audio_frames + num_frames, raw_audio_data_.begin() + current_size);

  // need to shift up by 1.0 (methinks)
  for (int i = 0; i < num_frames; i++){
    raw_audio_data_.push_back(audio_frames[i] + 2.0);
  }
}

void Features::pushFeatures()
{
  assert(current_features_ == NULL);

  if (pitches_.size() > 0){
    current_features_ = new TheFeatures();
    calcFeatures();
    saved_features_.push_back(*current_features_);

    reset();
  }
}

void Features::writeFeatures()
{
  std::ofstream fout;
  fout.open("features.txt");

  std::vector<TheFeatures>::const_iterator it;
  for (it = saved_features_.begin(); it < saved_features_.end(); it++){
    if (it->empty_feature == true)
      fout << "####" << std::endl;

    else{
      fout << "1:" << it->pitch_mean << " ";
      fout << "2:" << it->pitch_range << " ";
      fout << "3:" << it->pitch_variance << " ";
      fout << "4:" << it->pitch_slope << " ";
      fout << "5:" << it->raw_mean << " ";
      fout << "6:" << it->raw_range << std::endl;
    }
  }

  fout.close();
  fout.clear();
}

// Used as a placeholder to signal start of a new utterance
void Features::startNewUtterance()
{
  current_features_ = new TheFeatures();
  current_features_->empty_feature = true;
  saved_features_.push_back(*current_features_);
  reset();
}

void Features::reset()
{
  if(current_features_ != NULL){
    delete current_features_;
    current_features_ = NULL;
  }

  pitches_.clear();
  raw_audio_data_.clear();
}

void Features::calcFeatures()
{
  current_features_->pitch_mean = getPMean();
  current_features_->pitch_range = getPRange();
  current_features_->pitch_variance = getPVariance();
  current_features_->pitch_slope = getPSlope();
  current_features_->raw_mean = getRMean();
  current_features_->raw_range = getRRange();
}

float Features::getPMean()
{
  float mean = 0;

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++)
    mean += *it;

  mean /= pitches_.size();

  return mean;
}

float Features::getPRange()
{
  float min = 9999;
  float max = 0;

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    if (*it > max)
      max = *it;

    if (*it < min)
      min = *it;
  }

  float range = max - min;
  return range;
}

float Features::getPVariance()
{
  float variance = 0;
  float mean = getPMean();

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    float diff = mean - *it;
    diff *= diff; // square difference
    variance += diff;
  }

  variance /= pitches_.size(); // take the average
  return variance;
}

float Features::getPSlope()
{
  double* pitch_data = &pitches_[0];

  int number_of_data_points = pitches_.size();
  double xmatrix_[number_of_data_points * 2];

  // generate array like this: [1,1,1,2,1,3,1,4,1,5,...]
  for (int i = 0; i < number_of_data_points * 2; i++){
    if (i % 2 == 0) // even
      xmatrix_[i] = 1;

    else //odd
      xmatrix_[i] = (i + 1) / 2;
  }

  // dbg
  // std::cout << "y: \n";
  // for(int i = 0; i < number_of_data_points; i++)
  //   std::cout << pitch_data[i] << std::endl;

  // std::cout << "x: \n";
  // for(int i = 0; i < number_of_data_points*2; i++)
  //   std::cout << xmatrix_[i] << std::endl;
  //

  alglib::real_2d_array xmatrix;
  alglib::real_1d_array y;

  xmatrix.setcontent(number_of_data_points, 2, xmatrix_);
  y.setcontent(number_of_data_points, pitch_data);

  alglib::ae_int_t info;
  alglib::real_1d_array c;
  alglib::lsfitreport rep;

  //
  // Linear fitting without weights
  //
  alglib::lsfitlinear(y, xmatrix, info, c, rep);

  //std::cout << c[0] << ", " << c[1] << std::endl;

  return c[1];
}

float Features::getRMean()
{
  float mean = 0;

  std::vector<double>::const_iterator it;
  for (it = raw_audio_data_.begin(); it < raw_audio_data_.end(); it++)
    mean += *it;

  mean /= raw_audio_data_.size();

  return mean;
}

float Features::getRRange()
{
  float min = 9999;
  float max = 0;

  std::vector<double>::const_iterator it;
  for (it = raw_audio_data_.begin(); it < raw_audio_data_.end(); it++){
    if (*it > max)
      max = *it;

    if (*it < min)
      min = *it;
  }

  float range = max - min;
  return range;
}

