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
#include <numeric>

namespace pammap {
namespace tests {

TEST_CASE("ArrayView", "[array]") {
  using value_type = double;

  std::vector<value_type> data(1000);
  std::iota(data.begin(), data.end(), value_type(0));
  std::vector<size_t> shape{2, 50, 10};
  std::vector<ptrdiff_t> cc_strides{500, 10, 1};
  std::vector<ptrdiff_t> fortran_strides{1, 2, 100};

  ArrayView<value_type> cc{data.data(), shape, cc_strides};
  ArrayView<value_type> fortran{data.data(), shape, fortran_strides};
  const ArrayView<value_type> cc_const(cc);
  const ArrayView<value_type> fortran_const(fortran);

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

  SECTION("is_contiguous functions with unit stride") {
    ArrayView<value_type> aod{data.data(), {1000}, {1}};

    CHECK(fortran.is_fortran_contiguous());
    CHECK_FALSE(fortran.is_c_contiguous());

    CHECK_FALSE(cc.is_fortran_contiguous());
    CHECK(cc.is_c_contiguous());

    CHECK(aod.is_fortran_contiguous());
    CHECK(aod.is_c_contiguous());
  }

  SECTION("is_contiguous with non-unit stride") {
    // TODO
  }

  SECTION("Operator[] with unit stride") {
    CHECK(cc.size() == 1000);
    CHECK(fortran.size() == 1000);
#ifndef NDEBUG
    CHECK_THROWS_AS(cc[1000], IndexError);
    CHECK_THROWS_AS(fortran[1000], IndexError);
#endif  // NDEBUG

    // Check the C-ordered array (row-major)
    bool c_order_agrees = true;
    for (size_t i = 0, cnt = 0; i < shape[0]; ++i) {
      for (size_t j = 0; j < shape[1]; ++j) {
        const size_t ij = i * shape[1] + j;
        for (size_t k = 0; k < shape[2]; ++k, ++cnt) {
          const size_t ijk = ij * shape[2] + k;
          if (data[cnt] != cc[ijk]) c_order_agrees = false;
          if (data[cnt] != cc_const[ijk]) c_order_agrees = false;
        }
      }
    }
    CHECK(c_order_agrees);

    // Check the Fortran-ordered array (column-major)
    bool f_order_agrees = true;
    for (size_t k = 0, cnt = 0; k < shape[2]; ++k) {
      for (size_t j = 0; j < shape[1]; ++j) {
        for (size_t i = 0; i < shape[0]; ++i, ++cnt) {
          const size_t ijk = i + shape[0] * j + shape[0] * shape[1] * k;
          if (data[cnt] != fortran[ijk]) f_order_agrees = false;
          if (data[cnt] != fortran_const[ijk]) f_order_agrees = false;
        }
      }
    }
    CHECK(f_order_agrees);
  }

  SECTION("Operator[] with non-unit stride") {
    // TODO
  }

  SECTION("Setting values using operator[]") {
    // TODO
  }

  // TODO test equality operators
  // TODO test slice function
}

}  // namespace tests
}  // namespace pammap
