%module pammap_core

%{
#define SWIG_FILE_WITH_INIT
%}

%init %{
// Setup import of numpy arrays
import_array();
%}

%include "PamMapInterface.i"

// vi: syntax=c
