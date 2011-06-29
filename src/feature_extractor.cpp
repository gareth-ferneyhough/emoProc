#include "feature_extractor.h"

#include <typeinfo>

FeatureExtractor::FeatureExtractor() :
    logger_(Logger::getInstance()),
    my_name_(typeid(*this).name())
{
    //ctor
}

FeatureExtractor::~FeatureExtractor()
{
    //dtor
}

