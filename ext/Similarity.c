// TODO: add Dice's coefficient http://en.wikipedia.org/wiki/Dice's_coefficient
// TODO: add Sørensen index: http://en.wikipedia.org/wiki/Sørensen_similarity_index
// TODO: add Jaro-Winkler distance: http://www.census.gov/geo/msb/stand/strcmp.c#Original


#include "ruby.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

struct array_comparison {
  long intersection_size;
  long union_size;
  long a_size;
  long b_size;
};

typedef double (* fun_array_cmp)(const struct array_comparison);

static VALUE compare_arrays(VALUE a, VALUE b, fun_array_cmp cmp) {
  struct array_comparison result = { 0, 0, RARRAY(a)->len, RARRAY(b)->len };
  long * long_a;
  long * long_b;
  int i, j;
  
  result.union_size = result.a_size + result.b_size;
  
  if((result.a_size > 0) && (result.b_size > 0))
  {
    COPYRUBYHASHARRAY(a, long_a);
    COPYRUBYHASHARRAY(b, long_b);
    
    for(i = 0; i < result.a_size; ++i)
    {
      for(j = 0; j < result.b_size; ++j)
      {
        if(long_a[i] == long_b[j])
        {
          result.intersection_size++;
        }
      }
    }
    
  }
  
  return rb_float_new((*cmp)(result));
}

double tanimoto_coefficient(const struct array_comparison cmp) {
  if(cmp.union_size > 0)
  {
    return (cmp.intersection_size / (double)(cmp.union_size - cmp.intersection_size));
  }
  else
  {
    return 0.0;
  }
}

double dice_coefficient(const struct array_comparison cmp) {
  if(cmp.union_size > 0)
  {
    return (2 * cmp.intersection_size) / (double)(cmp.a_size + cmp.b_size);
  }
  else
  {
    return 0.0;
  }
}

// Calculates the Tanimoto coefficient between two sets.
static VALUE Similarity_tanimoto_coefficient(VALUE self, VALUE data1, VALUE data2) {
  return compare_arrays(data1, data2, tanimoto_coefficient);
}


static VALUE Similarity_dice_coefficient(VALUE self, VALUE data1, VALUE data2) {
  return compare_arrays(data1, data2, dice_coefficient);
}

VALUE rbgsl_mSimilarity;
void Init_Similarity() {
  rbgsl_mSimilarity = rb_define_module_under(rbgsl_mGSL, "Similarity");

  rb_define_module_function(rbgsl_mSimilarity, "tanimoto_coefficient", Similarity_tanimoto_coefficient, 2);
  rb_define_module_function(rbgsl_mSimilarity, "dice_coefficient", Similarity_dice_coefficient, 2);
}