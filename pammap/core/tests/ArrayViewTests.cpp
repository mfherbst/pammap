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
#include <catch2/catch.hpp>

namespace pammap {
namespace tests {

TEST_CASE("ArrayView tests", "[array]") {
  typedef double value_type;

  SECTION("Correct dimensionality.") {
    std::vector<value_type> data{1, 2, 3, 4};
    std::vector<size_t> shape{2, 2};
    std::vector<ptrdiff_t> strides{1, 2};

    ArrayView<value_type> av{data.data(), shape, strides};
    REQUIRE(av.size() == data.size());
  }

  SECTION("Dimensionality mismatch.") {
    auto construct = [] {
      std::vector<value_type> data{1, 2, 3, 4};
      std::vector<size_t> shape{2};
      std::vector<ptrdiff_t> strides{1, 2};

      ArrayView<value_type> av(data.data(), shape, strides);
    };

    REQUIRE_THROWS_AS(construct(), ValueError);
  }

  SECTION("Check is_contiguous functions") {
    std::vector<value_type> data(1000, 1);
    std::vector<size_t> shape{2, 50, 10};
    std::vector<ptrdiff_t> c_strides{500, 10, 1};
    std::vector<ptrdiff_t> fortran_strides{1, 2, 100};

    ArrayView<value_type> ac{data.data(), shape, c_strides};
    ArrayView<value_type> af{data.data(), shape, fortran_strides};
    ArrayView<value_type> aod{data.data(), {1000}, {1}};

    CHECK(af.is_fortran_contiguous());
    CHECK_FALSE(af.is_c_contiguous());

    CHECK_FALSE(ac.is_fortran_contiguous());
    CHECK(ac.is_c_contiguous());

    CHECK(aod.is_fortran_contiguous());
    CHECK(aod.is_c_contiguous());
  }
}

}  // namespace tests
}  // namespace pammap
