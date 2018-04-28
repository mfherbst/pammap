// vi: syntax=c
%module potential

%{
#include "potential.hpp"
#define SWIG_FILE_WITH_INIT
%}

%init %{
// Setup import of numpy arrays
import_array();
%}

%include "PamMap.i"
%include "potential.hpp"
