#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "settings_mgr.h"

MyFeatureExtractor::MyFeatureExtractor() :
    FeatureExtractor(),
    pitch(NULL)
{
    //ctor
    init();
}

void MyFeatureExtractor::init()
{
    pitch = new Pitch(SettingsMgr::getInstance()->getSampleRate());
}

MyFeatureExtractor::~MyFeatureExtractor()
{
    //dtor
}

void MyFeatureExtractor::processAudioSampleFunction(const float* const audio_frames, int num_frames)
{
    std::cout << pitch->getPitch(audio_frames, num_frames) << std::endl;
}
