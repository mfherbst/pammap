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

#include "ArrayView.hpp"
#include "exceptions.hpp"

namespace pammap {

ArrayViewBase::ArrayViewBase(std::vector<size_t> shape_, std::vector<size_t> strides_)
      : shape(shape_), strides(strides_) {
  pammap_throw(shape_.size() != strides_.size(), ValueError,
               "Size of shape vector (== " + std::to_string(shape_.size()) +
                     ") does not agree with the size of the strides vector "
                     "(== " +
                     std::to_string(strides_.size()) + ").");
}

}  // namespace pammap

#include "ArrayView.instantiation.hxx"
