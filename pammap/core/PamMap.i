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

//
// Do not edit. This file has been automatically generated by the script
// generate_dtype_code.py
// Instead edit the script and rerun it.
//

%{
#include "PamMap.hpp"
%}

%include "DataBlock.i"
%include "std_string.i"
%include "stdint.i"
%include "typedefs.hxx"

%apply (pammap::DataBlock<pammap::Complex> DATAVIEW) {(pammap::DataBlock<pammap::Complex> view)}
%apply (pammap::DataBlock<pammap::Integer> DATAVIEW) {(pammap::DataBlock<pammap::Integer> view)}
%apply (pammap::DataBlock<pammap::Float> DATAVIEW) {(pammap::DataBlock<pammap::Float> view)}

%include "PamMap.hpp"
%extend pammap::PamMap {
  void update_integer(std::string key, pammap::Complex value) {
    $self->update(key, std::move(value));
  }
  pammap::Complex get_complex(std::string key) {
    return $self->at<pammap::Complex>(key);
  }

  void update_integer(std::string key, pammap::Integer value) {
    $self->update(key, std::move(value));
  }
  pammap::Integer get_integer(std::string key) {
    return $self->at<pammap::Integer>(key);
  }

  void update_integer(std::string key, pammap::Float value) {
    $self->update(key, std::move(value));
  }
  pammap::Float get_float(std::string key) {
    return $self->at<pammap::Float>(key);
  }

  void update_integer(std::string key, pammap::String value) {
    $self->update(key, std::move(value));
  }
  pammap::String get_string(std::string key) {
    return $self->at<pammap::String>(key);
  }

void update_datablock_complex_view(std::string key, pammap::DataBlock<pammap::Complex> view) {
  $self->update(key, std::move(view));
}

void update_datablock_complex_copy(std::string key, pammap::DataBlock<pammap::Complex> view) {
  // Make a copy of the view including the memory
  pammap::DataBlock<pammap::Complex> copy(view, pammap::Memory::OwnCopy);
  $self->update(key, std::move(copy));
}

pammap::DataBlock<pammap::Complex> get_datablock_complex_copy(std::string key) {
  return $self->at<pammap::DataBlock<pammap::Complex>>(key);}

void update_datablock_integer_view(std::string key, pammap::DataBlock<pammap::Integer> view) {
  $self->update(key, std::move(view));
}

void update_datablock_integer_copy(std::string key, pammap::DataBlock<pammap::Integer> view) {
  // Make a copy of the view including the memory
  pammap::DataBlock<pammap::Integer> copy(view, pammap::Memory::OwnCopy);
  $self->update(key, std::move(copy));
}

pammap::DataBlock<pammap::Integer> get_datablock_integer_copy(std::string key) {
  return $self->at<pammap::DataBlock<pammap::Integer>>(key);}

void update_datablock_float_view(std::string key, pammap::DataBlock<pammap::Float> view) {
  $self->update(key, std::move(view));
}

void update_datablock_float_copy(std::string key, pammap::DataBlock<pammap::Float> view) {
  // Make a copy of the view including the memory
  pammap::DataBlock<pammap::Float> copy(view, pammap::Memory::OwnCopy);
  $self->update(key, std::move(copy));
}

pammap::DataBlock<pammap::Float> get_datablock_float_copy(std::string key) {
  return $self->at<pammap::DataBlock<pammap::Float>>(key);}

}