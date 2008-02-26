/* $Id: Sort.c,v 1.2 2003/07/14 08:14:19 pernici Exp $ */
#include "ruby.h"
#include "gsl/gsl_sort_vector.h"
#include "gsl/gsl_permutation.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Sorting vectors */

static VALUE Sort_sort_bang(VALUE obj) {
  gsl_vector * a;
  Data_Get_Struct(obj, gsl_vector, a);
  gsl_sort_vector(a);
  return obj;
}
 
static VALUE Sort_sort_index(VALUE obj) {
  gsl_vector * a;
  VALUE perm;
  gsl_permutation * ptr;
  size_t n;
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  ptr = gsl_permutation_alloc(n);
  gsl_sort_vector_index (ptr, a);
  perm = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, ptr);
  return perm;
}


/* Selecting the k-th smallest or largest elements */

static VALUE Sort_sort_smallest(VALUE obj, VALUE vk) {
  gsl_vector * a;
  double * dest;
  size_t k, n;
  VALUE ary;
  k  = FIX2INT(vk);
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  dest = ALLOC_N(double, k);
  gsl_sort_vector_smallest(dest, k, a);
  COPYCARRAY(dest, ary, k);
  return ary;
}

static VALUE Sort_sort_largest(VALUE obj, VALUE vk) {
  gsl_vector * a;
  double * dest;
  size_t k, n;
  VALUE ary;
  k  = FIX2INT(vk);
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  dest = ALLOC_N(double, k);
  gsl_sort_vector_largest(dest, k, a);
  COPYCARRAY(dest, ary, k);
  return ary;
}

/* Selecting the k-th smallest or largest elements */

static VALUE Sort_smallest_index(VALUE obj, VALUE vk) {
  gsl_vector * a;
  size_t * dest;
  size_t k, n;
  VALUE ary;
  k  = FIX2INT(vk);
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  dest = ALLOC_N(size_t, k);
  gsl_sort_vector_smallest_index(dest, k, a);
  COPYCARRAY(dest, ary, k);
  return ary;
}
    
static VALUE Sort_largest_index(VALUE obj, VALUE vk) {
  gsl_vector * a;
  size_t * dest;
  size_t k, n;
  VALUE ary;
  k  = FIX2INT(vk);
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  dest = ALLOC_N(size_t, k);
  gsl_sort_vector_largest_index(dest, k, a);
  COPYCARRAY(dest, ary, k);
  return ary;
}

/* Module definition */
VALUE rbgsl_mSort;

void Init_Sort() {
   rbgsl_mSort = rb_define_module_under(rbgsl_mGSL, "Sort");
   
  rb_define_method(rbgsl_cVector, "sort!", Sort_sort_bang, 0);
  rb_define_method(rbgsl_cVector, "sort_index", Sort_sort_index, 0);
  rb_define_method(rbgsl_cVector, "sort_smallest", Sort_sort_smallest, 1);
  rb_define_method(rbgsl_cVector, "sort_largest", Sort_sort_largest, 1);

  rb_define_method(rbgsl_cVector, "smallest_index", Sort_smallest_index, 1);
  rb_define_method(rbgsl_cVector, "largest_index", Sort_largest_index, 1);
}      
