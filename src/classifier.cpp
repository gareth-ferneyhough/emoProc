#include "classifier.h"
#include "settings_mgr.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "svm/svm.h"

Classifier::Classifier()
{
  
}

Classifier::~Classifier()
{
}

std::string Classifier::fileToStr(FILE* f)
{
  fseek(f, 0, SEEK_END); // seek to end of file
  size_t size = ftell(f); // get current file pointer
  fseek(f, 0, SEEK_SET); // seek back to beginning of file

  char* buff = new char[size + 1];
  size_t read = fread(buff, sizeof(char), size, f);
  buff[size] = '\0';  

  fseek(f, 0, SEEK_SET); // seek back to beginning of file
 
  return std::string(buff);
}

std::string Classifier::scaleData(std::string utterance)
{
  // Convert string to c string and remove const-ness. Then open as memfile
  char* c_str_buffer = const_cast<char*>(utterance.c_str());
  FILE* mem_file = fmemopen(c_str_buffer, (utterance.length())*sizeof(char), "r");

  std::cout << "pre:\n" << fileToStr(mem_file);

  // scale
  FILE *scaled = fmemopen(NULL, 1000000, "r+");
  FILE *scale_params = fopen("../data/scale-params.txt", "r");
  svm_scale(mem_file, scaled, scale_params);

  std::cout << "scaled:\n" << fileToStr(scaled);

  fclose(mem_file);
  fclose(scaled);

  return "1";//scaled_string;  
}

  // // load model
  // struct svm_model *model;
  // const char *model_filename = SettingsMgr::getInstance()->getModelFilename().c_str();
  // if((model=svm_load_model(model_filename))==0)
  //   {
  //     fprintf(stderr,"can't open model file %s\n", model_filename);
  //     exit(1);
  //   }


Classifier* Classifier::getInstance()
{
  static Classifier instance;
  return &instance;
}
