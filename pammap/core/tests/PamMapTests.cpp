//
// Copyright (C) 2018 by the pammap authors
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

#include "PamMap.hpp"
#include "exceptions.hpp"
#include <catch2/catch.hpp>

namespace pammap {
namespace tests {

TEST_CASE("PamMap tests", "[pammap]") {
  // Some data:
  Integer i = 5;
  String s{"test"};
  Float f = 7.5;

  // Some vectors to point into
  std::vector<Float> fvec{1., 2., 3., 4.};
  std::vector<Integer> ivec{1, 2, 3, 4};
  ArrayView<Float> farr(fvec);
  ArrayView<Integer> iarr(ivec);

  SECTION("Can add data to PamMap") {
    // Insert some data into a map.
    PamMap m{};
    m.update("string", s);
    m.update("integer", i);

    // See if we get it back:
    REQUIRE(m.at<Integer>("integer") == i);
    REQUIRE(m.at<String>("string") == s);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check that type safety is assured") {
    // Add data to map.
    PamMap m{};
    m.update("s", s);
    m.update("i", i);
    m.update("farr", farr);

    // Extract using the wrong type
    REQUIRE_THROWS_AS(m.at<Float>("i"), TypeError);
    REQUIRE_THROWS_AS(m.at<Float>("s"), TypeError);
    REQUIRE_THROWS_AS(m.at<Float>("farr"), TypeError);
    REQUIRE(m.at<ArrayView<Float>>("farr") == farr);
    REQUIRE(m.at<const ArrayView<Float>>("farr") == farr);
  }

}  // TEST_CASE
}  // namespace tests
}  // namespace pammap
