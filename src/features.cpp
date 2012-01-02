#include <assert.h>
#include <fstream>
#include <iostream>

#include "stdafx.h"
#include "interpolation.h"

#include "features.h"
#include "speech_energy.h"

Features::Features() :
  current_features_(NULL),
  count_(0),
  start_of_last_utterance_(0)
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
  fout.open("../data/features.txt");

  std::vector<TheFeatures>::const_iterator it;
  for (it = saved_features_.begin(); it < saved_features_.end(); it++){
    if (it->empty_feature == true)
      fout << "####" << std::endl;

    else{
      fout << "1:" << it->pitch_mean << " ";
      fout << "2:" << it->pitch_range << " ";
      fout << "3:" << it->pitch_variance << " ";
      fout << "4:" << it->pitch_slope << " ";
      fout << "5:" << it->speech_energy << " ";
      fout << "6:" << it->frame_length << std::endl;
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
  start_of_last_utterance_ = saved_features_.size();
  reset();
}

std::string Features::getLastUtteranceAsString() const
{
  std::string utterance_string;

  for(int i = start_of_last_utterance_; i < saved_features_.size(); ++i){
    utterance_string += "1 ";
    utterance_string += saved_features_[i].toString();
  }

  return utterance_string;
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
  current_features_->speech_energy = getSpeechEnergy();
  current_features_->frame_length = getFrameLength();
  //  current_features_->raw_mean = getRMean();
  //  current_features_->raw_range = getRRange();
}

float Features::getPMean()
{
  float mean = 0;
  int count = 0;

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    if (*it != 0){
      mean += *it;
      count++;
    }
  }
  mean /= count;

  return mean;
}

float Features::getPRange()
{
  float min = 9999;
  float max = 0;

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    if (*it != 0){
      if (*it > max)
        max = *it;

      if (*it < min)
        min = *it;
    }
  }

  float range = max - min;
  return range;
}

float Features::getPVariance()
{
  float variance = 0;
  float mean = getPMean();
  int count = 0;

  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    if (*it != 0){
      float diff = mean - *it;
      diff *= diff; // square difference
      variance += diff;
      count++;
    }
  }

  variance /= count; // take the average
  return variance;
}

float Features::getPSlope()
{
  // Remove zeros from pitches_
  std::vector<double> new_pitches;
  std::vector<double>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++){
    if (*it != 0)
      new_pitches.push_back(*it);
  }

  double* pitch_data = &new_pitches[0]; //&pitches_[0];

  int number_of_data_points = new_pitches.size(); //pitches_.size();
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

float Features::getSpeechEnergy()
{
  double sum_of_squares = 0;

  std::vector<double>::const_iterator it;
  for (it = raw_audio_data_.begin(); it < raw_audio_data_.end(); it++)
    sum_of_squares += (*it) * (*it);

  double speech_energy = sum_of_squares / (float)raw_audio_data_.size();

  return (float)speech_energy;

  //  return SpeechEnergy::getSpeechEnergy((float*)&raw_audio_data_[0], raw_audio_data_.size());
}

float Features::getFrameLength()
{
  return raw_audio_data_.size();
}

// float Features::getRMean()
// {
//   float mean = 0;

//   std::vector<double>::const_iterator it;
//   for (it = raw_audio_data_.begin(); it < raw_audio_data_.end(); it++)
//     mean += *it;

//   mean /= raw_audio_data_.size();

//   return mean;
// }

// float Features::getRRange()
// {
//   float min = 9999;
//   float max = 0;

//   std::vector<double>::const_iterator it;
//   for (it = raw_audio_data_.begin(); it < raw_audio_data_.end(); it++){
//     if (*it > max)
//       max = *it;

//     if (*it < min)
//       min = *it;
//   }

//   float range = max - min;
//   return range;
// }

