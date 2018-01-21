%module pammap_core

%{
#include "PamMapInterface.hxx"
#define SWIG_FILE_WITH_INIT
%}

%init %{
// Setup import of numpy arrays
import_array();
%}

%include "DataBlock.i"
%include "std_string.i"
%include "stdint.i"
%include "typedefs.hxx"
%include "PamMapInterface.hxx"

// vi: syntax=c
