// TODO: add Dice's coefficient http://en.wikipedia.org/wiki/Dice's_coefficient
// TODO: add Sørensen index: http://en.wikipedia.org/wiki/Sørensen_similarity_index
// TODO: add Jaro-Winkler distance: http://www.census.gov/geo/msb/stand/strcmp.c#Original


#include "ruby.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

struct array_comparison {
  long intersection_size;
  long union_size;
};

static struct array_comparison compare_arrays(VALUE a, VALUE b) {
  size_t a_size = RARRAY(a)->len;
  size_t b_size = RARRAY(b)->len;
  struct array_comparison result;
  long * long_a;
  long * long_b;
  int i, j;
  
  result.intersection_size = 0;
  result.union_size = a_size + b_size;
  
  if((a_size > 0) && (b_size > 0))
  {
    COPYRUBYHASHARRAY(a, long_a);
    COPYRUBYHASHARRAY(b, long_b);
    
    for(i = 0; i < a_size; ++i)
    {
      for(j = 0; j < b_size; ++j)
      {
        if(long_a[i] == long_b[j])
        {
          result.intersection_size++;
        }
      }
    }
    
  }
  
  return result;
}

// Calculates the Tanimoto coefficient between two sets.
static VALUE Similarity_tanimoto_coefficient(VALUE self, VALUE data1, VALUE data2) {
  struct array_comparison cmp = compare_arrays(data1, data2);
  double result = 0.0;
  
  if(cmp.union_size > 0)
  {
    result = cmp.intersection_size / (double)(cmp.union_size - cmp.intersection_size);
  }
  
  return rb_float_new(result);
}



VALUE rbgsl_mSimilarity;
void Init_Similarity() {
  rbgsl_mSimilarity = rb_define_module_under(rbgsl_mGSL, "Similarity");

  rb_define_module_function(rbgsl_mSimilarity, "tanimoto_coefficient", Similarity_tanimoto_coefficient, 2);
}