#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "svm.h"

extern "C" int svm_scale(FILE*, FILE*, FILE*);

char *line = NULL;
int max_line_len = 1024;
double lower=-1.0,upper=1.0,y_lower,y_upper;
int y_scaling = 0;
double *feature_max;
double *feature_min;
double y_max = -DBL_MAX;
double y_min = DBL_MAX;
int max_index;
long int num_nonzeros = 0;
long int new_num_nonzeros = 0;

#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))

void output_target(double value, FILE*);
void output(int index, double value, FILE*);
char* readline(FILE *input);

int svm_scale(FILE* in, FILE* out, FILE* fp_restore)
{
  int i,index;

  if(in == NULL)
    {
      fprintf(stderr,"can't open file \n");
      exit(1);
    }

  bool restore_scaling_params = false;
  if(fp_restore != NULL)
    restore_scaling_params = true;

  line = (char *) malloc(max_line_len*sizeof(char));

#define SKIP_TARGET                             \
  while(isspace(*p)) ++p;                       \
  while(!isspace(*p)) ++p;

#define SKIP_ELEMENT                            \
  while(*p!=':') ++p;                           \
  ++p;                                          \
  while(isspace(*p)) ++p;                       \
  while(*p && !isspace(*p)) ++p;

  /* assumption: min index of attributes is 1 */
  /* pass 1: find out max index of attributes */
  max_index = 0;

  if(restore_scaling_params)
    {
      int idx, c;

      // fp_restore = fopen(restore_filename,"r");
      // if(fp_restore==NULL)
      //   {
      //     fprintf(stderr,"can't open file %s\n", restore_filename);
      //     exit(1);
      //   }

      c = fgetc(fp_restore);
      if(c == 'y')
        {
          readline(fp_restore);
          readline(fp_restore);
          readline(fp_restore);
        }
      readline(fp_restore);
      readline(fp_restore);

      while(fscanf(fp_restore,"%d %*f %*f\n",&idx) == 1)
        max_index = max(idx,max_index);
      rewind(fp_restore);
    }

  while(readline(in)!=NULL)
    {
      char *p=line;

      SKIP_TARGET

        while(sscanf(p,"%d:%*f",&index)==1)
          {
            max_index = max(max_index, index);
            SKIP_ELEMENT
              num_nonzeros++;
          }
    }
  rewind(in);

  feature_max = (double *)malloc((max_index+1)* sizeof(double));
  feature_min = (double *)malloc((max_index+1)* sizeof(double));

  if(feature_max == NULL || feature_min == NULL)
    {
      fprintf(stderr,"can't allocate enough memory\n");
      exit(1);
    }

  for(i=0;i<=max_index;i++)
    {
      feature_max[i]=-DBL_MAX;
      feature_min[i]=DBL_MAX;
    }

  /* pass 2: find out min/max value */
  while(readline(in)!=NULL)
    {
      char *p=line;
      int next_index=1;
      double target;
      double value;

      sscanf(p,"%lf",&target);
      y_max = max(y_max,target);
      y_min = min(y_min,target);

      SKIP_TARGET

        while(sscanf(p,"%d:%lf",&index,&value)==2)
          {
            for(i=next_index;i<index;i++)
              {
                feature_max[i]=max(feature_max[i],0);
                feature_min[i]=min(feature_min[i],0);
              }

            feature_max[index]=max(feature_max[index],value);
            feature_min[index]=min(feature_min[index],value);

            SKIP_ELEMENT
              next_index=index+1;
          }

      for(i=next_index;i<=max_index;i++)
        {
          feature_max[i]=max(feature_max[i],0);
          feature_min[i]=min(feature_min[i],0);
        }
    }

  rewind(in);

  /* pass 2.5: save/restore feature_min/feature_max */

  if(restore_scaling_params)
    {
      /* fp_restore rewinded in finding max_index */
      int idx, c;
      double fmin, fmax;

      if((c = fgetc(fp_restore)) == 'y')
        {
          fscanf(fp_restore, "%lf %lf\n", &y_lower, &y_upper);
          fscanf(fp_restore, "%lf %lf\n", &y_min, &y_max);
          y_scaling = 1;
        }
      else
        ungetc(c, fp_restore);

      if (fgetc(fp_restore) == 'x') {
        fscanf(fp_restore, "%lf %lf\n", &lower, &upper);
        while(fscanf(fp_restore,"%d %lf %lf\n",&idx,&fmin,&fmax)==3)
          {
            if(idx<=max_index)
              {
                feature_min[idx] = fmin;
                feature_max[idx] = fmax;
              }
          }
      }
      fclose(fp_restore);
    }

  /* pass 3: scale */
  while(readline(in)!=NULL)
    {
      char *p=line;
      int next_index=1;
      double target;
      double value;

      sscanf(p,"%lf",&target);
      output_target(target, out);

      SKIP_TARGET

        while(sscanf(p,"%d:%lf",&index,&value)==2)
          {
            for(i=next_index;i<index;i++)
              output(i,0, out);

            output(index,value, out);

            SKIP_ELEMENT
              next_index=index+1;
          }

      for(i=next_index;i<=max_index;i++)
        output(i,0, out);

      fprintf(out, "\n");
    }

  if (new_num_nonzeros > num_nonzeros)
    fprintf(stderr,
            "Warning: original #nonzeros %ld\n"
            "         new      #nonzeros %ld\n"
            "Use -l 0 if many original feature values are zeros\n",
            num_nonzeros, new_num_nonzeros);

  free(line);
  free(feature_max);
  free(feature_min);

  return 0;
}

char* readline(FILE *input)
{
  int len;

  if(fgets(line,max_line_len,input) == NULL)
    return NULL;

  while(strrchr(line,'\n') == NULL)
    {
      max_line_len *= 2;
      line = (char *) realloc(line, max_line_len);
      len = (int) strlen(line);
      if(fgets(line+len,max_line_len-len,input) == NULL)
        break;
    }
  return line;
}

void output_target(double value, FILE *out)
{
  if(y_scaling)
    {
      if(value == y_min)
        value = y_lower;
      else if(value == y_max)
        value = y_upper;
      else value = y_lower + (y_upper-y_lower) *
             (value - y_min)/(y_max-y_min);
    }
  fprintf(out, "%g ",value);
}

void output(int index, double value, FILE *out)
{
  /* skip single-valued attribute */
  if(feature_max[index] == feature_min[index])
    return;

  if(value == feature_min[index])
    value = lower;
  else if(value == feature_max[index])
    value = upper;
  else
    value = lower + (upper-lower) *
      (value-feature_min[index])/
      (feature_max[index]-feature_min[index]);

  if(value != 0)
    {
      fprintf(out, "%d:%g ",index, value);
      new_num_nonzeros++;
    }
}
