// vi: syntax=c
%module swiginterface

%{
#include "swiginterface.hpp"
#define SWIG_FILE_WITH_INIT
%}

%init %{
// Setup import of numpy arrays
import_array();
%}

%include "PamMap.i"
%include "swiginterface.hpp"
