/*
* ruby-gsl - using GSL from Ruby
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Id: Block.c,v 1.2 2003/07/04 20:58:18 aerpenbeck Exp $ */

/*
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 1999-2001 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_block.h"
#include "ruby-gsl.h"

/* Blocks */

static VALUE Block_new(VALUE klass, VALUE nn) {
  VALUE obj;
  gsl_block *block;

  block = gsl_block_alloc(NUM2INT(nn));

  obj = Data_Wrap_Struct(klass, 0, gsl_block_free, block);

  return obj;
}

static VALUE Block_calloc(VALUE klass, VALUE nn) {
  VALUE obj;
  gsl_block *block;

  block = gsl_block_alloc(NUM2INT(nn));

  obj = Data_Wrap_Struct(klass, 0, gsl_block_free, block);

  return obj;
}

static VALUE Block_size(VALUE obj) {
  gsl_block *block;

  Data_Get_Struct(obj, gsl_block, block);

  return INT2FIX(block->size);
}

/* Reading and Writing Blocks */

static VALUE Block_fwrite(VALUE self, VALUE stream) {
  int ret;
  gsl_block * ptr;
  OpenFile *fptr;
  Data_Get_Struct(self, gsl_block, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_block_fwrite(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Block_fread(VALUE self, VALUE stream) {
  int ret;
  gsl_block * ptr;
  OpenFile *fptr;
  Data_Get_Struct(self, gsl_block, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_block_fread(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Block_fprintf(VALUE self, VALUE stream, VALUE format) {
  int ret;
  gsl_block * ptr;
  OpenFile *fptr;
  Data_Get_Struct(self, gsl_block, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_block_fprintf(fptr->f, ptr, STR2CSTR(format));
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Block_fscanf(VALUE self, VALUE stream) {
  int ret;
  gsl_block * ptr;
  OpenFile *fptr;
  Data_Get_Struct(self, gsl_block, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_block_fscanf(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

/* Class definition */

VALUE rbgsl_cBlock;

void Init_Block() {
  rbgsl_cBlock = rb_define_class_under(rbgsl_mGSL, "Block", rb_cObject);
  
  rb_define_singleton_method(rbgsl_cBlock, "new", Block_new, 1);
  rb_define_singleton_method(rbgsl_cBlock, "alloc", Block_new, 1);
  rb_define_singleton_method(rbgsl_cBlock, "calloc", Block_calloc, 1);
  
  rb_define_method(rbgsl_cBlock, "size", Block_size, 0);
  
  rb_define_method(rbgsl_cBlock, "fwrite", Block_fwrite, 1);
  rb_define_method(rbgsl_cBlock, "fread", Block_fread, 1);
  rb_define_method(rbgsl_cBlock, "fprintf", Block_fprintf, 2);
  rb_define_method(rbgsl_cBlock, "fscanf", Block_fscanf, 1);
}
/* vim: set ts=4: */
