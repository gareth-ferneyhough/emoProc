#include "features.h"
#include <assert.h>
#include <fstream>

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
  pitches_.push_back(pitch);
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

void Features::reset()
{
  if(current_features_ != NULL){
    delete current_features_;
    current_features_ = NULL;
  }

  pitches_.clear();
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

  std::vector<float>::const_iterator it;
  for (it = pitches_.begin(); it < pitches_.end(); it++)
    mean += *it;

  mean /= pitches_.size();

  return mean;
}

float Features::getPRange()
{
  float min = 9999;
  float max = 0;

  std::vector<float>::const_iterator it;
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

  std::vector<float>::const_iterator it;
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
  // still need to implement some kind of curve fitting
  return 1.0;
}

float Features::getRMean()
{
  return -1;
}

float Features::getRRange()
{
  return -1;
}

void Features::writeFeatures()
{
  std::ofstream fout;
  fout.open("features.txt");

  std::vector<TheFeatures>::const_iterator it;
  for (it = saved_features_.begin(); it < saved_features_.end(); it++){
    fout << "1:" << it->pitch_mean << " ";
    fout << "2:" << it->pitch_range << " ";
    fout << "3:" << it->pitch_variance << " ";
    fout << "4:" << it->pitch_slope << " ";
    fout << "5:" << it->raw_mean << " ";
    fout << "6:" << it->raw_range << std::endl;
  }

  fout.close();
  fout.clear();
}
