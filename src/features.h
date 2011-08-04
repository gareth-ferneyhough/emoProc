#ifndef FEATURES_H
#define FEATURES_H

#include <vector>

class TheFeatures;

class Features
{
 public:
  Features();
  ~Features();
  
  void savePitch(float pitch);
  void pushFeatures();
  void writeFeatures();

 private:
  void reset();
  void calcFeatures();
  float getPMean();
  float getPRange();
  float getPVariance();
  float getPSlope();
  float getRMean();
  float getRRange();

  std::vector<double> pitches_; // double only necessary for easy use with alglib
  std::vector<TheFeatures> saved_features_;
  TheFeatures* current_features_;
  int count_;
};

class TheFeatures
{
 public:
 TheFeatures() :
  pitch_mean(0),
  pitch_range(0),
  pitch_variance(0),
  pitch_slope(0),
  raw_mean(0),
  raw_range(0)
  {}

  float pitch_mean;
  float pitch_range;
  float pitch_variance;
  float pitch_slope;
  float raw_mean;
  float raw_range;
};

#endif // FEATURES_H
