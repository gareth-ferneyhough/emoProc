#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "svm.h"

// forgive me
extern "C" int my_svm_predict(FILE*, FILE*, struct svm_model* );

struct svm_node *x;
int max_nr_attr = 64;

int predict_probability=0;

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
  int len;

  if(fgets(line,max_line_len,input) == NULL)
    return NULL;

  while(strrchr(line,'\n') == NULL)
    {
      max_line_len *= 2;
      line = (char *) realloc(line,max_line_len);
      len = (int) strlen(line);
      if(fgets(line+len,max_line_len-len,input) == NULL)
        break;
    }
  return line;
}

void exit_input_error(int line_num)
{
  fprintf(stderr,"Wrong input format at line %d\n", line_num);
  exit(1);
}

void predict(FILE *input, FILE *output, struct svm_model *model)
{
  int correct = 0;
  int total = 0;
  double error = 0;
  double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

  int svm_type=svm_get_svm_type(model);
  int nr_class=svm_get_nr_class(model);
  double *prob_estimates=NULL;
  int j;

  if(predict_probability)
    {
      if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
        printf("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
      else
        {
          int *labels=(int *) malloc(nr_class*sizeof(int));
          svm_get_labels(model,labels);
          prob_estimates = (double *) malloc(nr_class*sizeof(double));
          fprintf(output,"labels");
          for(j=0;j<nr_class;j++)
            fprintf(output," %d",labels[j]);
          fprintf(output,"\n");
          free(labels);
        }
    }

  max_line_len = 1024;
  line = (char *)malloc(max_line_len*sizeof(char));
  while(readline(input) != NULL)
    {
      int i = 0;
      double target_label, predict_label;
      char *idx, *val, *label, *endptr;
      int inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0

      label = strtok(line," \t\n");
      if(label == NULL){ // empty line
        // printf("emptyline\n");
        break;
        // exit_input_error(total+1);
      }

      target_label = strtod(label,&endptr);
      if(endptr == label || *endptr != '\0')
        exit_input_error(total+1);

      while(1)
        {
          if(i>=max_nr_attr-1)  // need one more for index = -1
            {
              max_nr_attr *= 2;
              x = (struct svm_node *) realloc(x,max_nr_attr*sizeof(struct svm_node));
            }

          idx = strtok(NULL,":");
          val = strtok(NULL," \t");

          if(val == NULL)
            break;
          errno = 0;
          x[i].index = (int) strtol(idx,&endptr,10);
          if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
            exit_input_error(total+1);
          else
            inst_max_index = x[i].index;

          errno = 0;
          x[i].value = strtod(val,&endptr);
          if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
            exit_input_error(total+1);

          ++i;
        }
      x[i].index = -1;

      if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
        {
          predict_label = svm_predict_probability(model,x,prob_estimates);
          fprintf(output,"%g",predict_label);
          for(j=0;j<nr_class;j++)
            fprintf(output," %g",prob_estimates[j]);
          fprintf(output,"\n");
        }
      else
        {
          predict_label = svm_predict(model,x);
          fprintf(output,"%g\n",predict_label);
        }

      if(predict_label == target_label)
        ++correct;
      error += (predict_label-target_label)*(predict_label-target_label);
      sump += predict_label;
      sumt += target_label;
      sumpp += predict_label*predict_label;
      sumtt += target_label*target_label;
      sumpt += predict_label*target_label;
      ++total;
    }
  if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
    {
      printf("Mean squared error = %g (regression)\n",error/total);
      printf("Squared correlation coefficient = %g (regression)\n",
             ((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
             ((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt))
             );
    }
  else
    /*printf("Accuracy = %g%% (%d/%d) (classification)\n",
      (double)correct/total*100,correct,total); */
  if(predict_probability)
    free(prob_estimates);
}

int my_svm_predict(FILE *input, FILE *output, struct svm_model *model)
{
  int i;

  x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));

  predict(input,output, model);
  free(x);
  free(line);

  return 0;
}
