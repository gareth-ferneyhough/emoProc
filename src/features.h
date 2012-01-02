#ifndef FEATURES_H
#define FEATURES_H

#include <vector>
#include <sstream>
#include <string>

class TheFeatures;

class Features
{
 public:
  Features();
  ~Features();

  void savePitch(float pitch);
  void saveRaw(const float* const audio_frames, int num_frames);
  void pushFeatures();
  void writeFeatures();
  void startNewUtterance();
  std::string getLastUtteranceAsString() const;

 private:
  void reset();
  void calcFeatures();
  float getPMean();
  float getPRange();
  float getPVariance();
  float getPSlope();
  float getSpeechEnergy();
  float getFrameLength();
  //  float getRMean();
  //  float getRRange();

  std::vector<double> pitches_; // double only necessary for easy use with alglib
  std::vector<double> raw_audio_data_;     // raw audio data
  std::vector<TheFeatures> saved_features_;
  TheFeatures* current_features_;
  int count_;
  int start_of_last_utterance_;
  
};

class TheFeatures
{
 public:
 TheFeatures() :
  pitch_mean(0),
    pitch_range(0),
    pitch_variance(0),
    pitch_slope(0),
    speech_energy(0),
    frame_length(0),
    empty_feature(false)
      {}

  const std::string toString() const
  {
    std::stringstream ss;

    ss << "1:" << pitch_mean << " ";
    ss << "2:" << pitch_range << " ";
    ss << "3:" << pitch_variance << " ";
    ss << "4:" << pitch_slope << " ";
    ss << "5:" << speech_energy << " ";
    ss << "6:" << frame_length  << " ";
    //    ss << "5:" << raw_mean << " ";
    //    ss << "6:" << raw_range << std::endl;

    return ss.str();
  }

  float pitch_mean;
  float pitch_range;
  float pitch_variance;
  float pitch_slope;
  //  float raw_mean;
  //  float raw_range;
  float speech_energy;
  float frame_length;
  bool empty_feature;
};

#endif // FEATURES_H
