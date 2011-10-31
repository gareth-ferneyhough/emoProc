#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>

#include "uncopyable.h"

extern "C" int svm_scale(FILE* in, FILE* out);
extern "C" int my_svm_predict(FILE* in, FILE* out, struct svm_model* model);

class Classifier
{
 public:
  static Classifier* getInstance();
  std::string scaleData(std::string utterance);
  std::string fileToStr(FILE*);

 private:
  DISALLOW_COPY_AND_ASSIGN(Classifier);
  Classifier();
  ~Classifier();
};

#endif // CLASSIFIER_H
