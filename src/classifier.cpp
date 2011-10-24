#include "classifier.h"

#include <stdlib.h>
#include <stdio.h>

#include "svm/svm.h"

Classifier::Classifier()
{
  
}

Classifier::~Classifier()
{
}

std::string Classifier::scaleData(std::string utterance)
{
  char* c_str_buffer = const_cast<char*>(utterance.c_str());
  FILE* mem_file = fmemopen(c_str_buffer, (utterance.length() + 1)*sizeof(char), "r");

  fclose(mem_file);

  return "hi";
  
}

Classifier* Classifier::getInstance()
{
  static Classifier instance;
  return &instance;
}
