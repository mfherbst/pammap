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
#include "exceptions.hpp"
%}

%exception {
  try {
    $action
  } catch (const pammap::NotImplementedError& e) {
    PyErr_SetString(PyExc_NotImplementedError, e.what());
    return NULL;
  } catch (const pammap::ValueError& e) {
    PyErr_SetString(PyExc_ValueError, e.extra.c_str());
    return NULL;
  } catch (const pammap::KeyError& e) {
    PyErr_SetString(PyExc_KeyError, e.key.c_str());
    return NULL;
  } catch (const pammap::PamMapError& e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}
