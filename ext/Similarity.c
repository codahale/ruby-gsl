// TODO: add Dice's coefficient http://en.wikipedia.org/wiki/Dice's_coefficient
// TODO: add Sørensen index: http://en.wikipedia.org/wiki/Sørensen_similarity_index
// TODO: add Jaro-Winkler distance: http://www.census.gov/geo/msb/stand/strcmp.c#Original


#include "ruby.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

// Returns the number of elements two arrays have in common.
static long intersection_size(long * a, size_t a_size, long * b, size_t b_size) {
  size_t i, j, result = 0;
  for(i = 0; i < a_size; ++i)
  {
    for(j = 0; j < b_size; ++j)
    {
      if(a[i] == b[j])
      {
        result++;
      }
    }
  }
  return result;
}

// Calculates the Tanimoto coefficient between two sets.
static VALUE Similarity_tanimoto_coefficient(VALUE self, VALUE data1, VALUE data2) {
  long * my_data1;
  long * my_data2;
  size_t size1 = RARRAY(data1)->len,
         size2 = RARRAY(data2)->len,
         union_size = size1 + size2;
  
  // Bail if either of the arrays are zero-length.
  if((size1 == 0) || (size2 == 0))
  {
    return rb_float_new(0);
  }
  
  // Create two arrays of the elements' hashes. (Which should be relatively
  // unique.)
  COPYRUBYHASHARRAY(data1, my_data1);
  COPYRUBYHASHARRAY(data2, my_data2);
  
  // Return the Tanimoto coefficient.
  long int_size = intersection_size(my_data1, size1, my_data2, size2);
  return rb_float_new(int_size / (double)(union_size - int_size));
}

VALUE rbgsl_mSimilarity;
void Init_Similarity() {
  rbgsl_mSimilarity = rb_define_module_under(rbgsl_mGSL, "Similarity");

  rb_define_module_function(rbgsl_mSimilarity, "tanimoto_coefficient", Similarity_tanimoto_coefficient, 2);
}