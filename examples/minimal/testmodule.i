// vi: syntax=c
%module testmodule

%{
#include "testmodule.hpp"
#define SWIG_FILE_WITH_INIT
%}

%init %{
// Setup import of numpy arrays
import_array();
%}

%include "PamMap.i"
%include "testmodule.hpp"
