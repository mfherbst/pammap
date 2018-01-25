// vi: syntax=c
//
// Copyright (C) 2018 by Michael F. Herbst and contributors
//
// This file is part of pammap.
//
// pammap is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// pammap is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with pammap. If not, see <http://www.gnu.org/licenses/>.
//

%{
#include "DataBlock.hpp"
#include "typedefs.hxx"
%}

%include "numpy.i"

/*
 *  DataBlock typemaps
 */
%define %datablock_typemaps(DATA_TYPE, DATA_TYPECODE)
%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY,fragment="NumPy_Macros")
  (DATA_TYPE DATAVIEW)
{
  $1 = is_array($input) && PyArray_EquivTypenums(array_type($input),
                                                 DATA_TYPECODE);
}

/** Typemap to pass a view to a numpy array to C++
  */
%typemap(in,fragment="NumPy_Fragments")
  (DATA_TYPE DATAVIEW)
  (PyArrayObject* array=NULL)
{
  array = obj_to_array_no_conversion($input, DATA_TYPECODE);
  if (!array || !require_c_or_f_contiguous(array)
      || !require_native(array)) SWIG_fail;

  std::vector<size_t> shape(array_numdims(array));
  for (int i=0; i < array_numdims(array); ++i) {
    shape[i] = array_size(array,i);
  }

  // TODO Assign strides as well!
  $1 = DATA_TYPE(
           static_cast<typename DATA_TYPE::value_type*>(array_data(array)),
           shape,
           pammap::Memory::ViewOnly);

}

%typemap(out,fragment="NumPy_Fragments")
  (DATA_TYPE)
  (PyObject* array = NULL)
{
  std::vector<npy_intp> np_shape($1.shape().size());
  for (size_t i = 0; i < np_shape.size(); ++i) {
    np_shape[i] = static_cast<npy_intp>($1.shape()[i]);
  }
  array = PyArray_SimpleNew(np_shape.size(), np_shape.data(), DATA_TYPECODE);
  if (!array) SWIG_fail;

  std::copy($1.begin(), $1.end(),
            static_cast<typename DATA_TYPE::value_type*>(array_data(array)));

  $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
}
%enddef

//
// Do not use the bare *.i.template file, use the generated DataBlock.i
// files instead.
// In the full version the instantiation typemaps will be added here.
//

%datablock_typemaps(pammap::DataBlock<pammap::Complex>, NPY_CDOUBLE)
%datablock_typemaps(pammap::DataBlock<pammap::Integer>, NPY_LONGLONG)
%datablock_typemaps(pammap::DataBlock<pammap::Float>, NPY_DOUBLE)