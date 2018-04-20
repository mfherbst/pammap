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

#include "Slice.hpp"
#include "exceptions.hpp"
#include <catch2/catch.hpp>

namespace pammap {

TEST_CASE("Slice tests", "[slice]") {
  typedef std::tuple<ptrdiff_t, ptrdiff_t, ptrdiff_t> idcs_t;

  SECTION("Test basic constructors.") {
    Slice def{};
    CHECK(def.begin == Auto);
    CHECK(def.end == Auto);
    CHECK(def.step == 1);

    Slice one{42};
    CHECK(one.begin == 42);
    CHECK(one.end == 43);
    CHECK(one.step == 1);

    Slice two{3, 7};
    CHECK(two.begin == 3);
    CHECK(two.end == 7);
    CHECK(two.step == 1);

    Slice three{7, 3, -1};
    CHECK(three.begin == 7);
    CHECK(three.end == 3);
    CHECK(three.step == -1);
  }

  SECTION("Zero step not allowed.") {
    REQUIRE_THROWS_AS([&]() { Slice test(1, 2, 0); }(), ValueError);
  }

  SECTION("Check computed indices for positive step.") {
    idcs_t all = All.indices(15);
    CHECK(std::get<0>(all) == 0);
    CHECK(std::get<1>(all) == 15);
    CHECK(std::get<2>(all) == 1);

    for (ptrdiff_t step = 1; step < 5; ++step) {
      idcs_t second1 = Slice{Auto, 8, step}.indices(13);
      CHECK(std::get<0>(second1) == 0);
      CHECK(std::get<1>(second1) == 8);
      CHECK(std::get<2>(second1) == step);

      idcs_t second2 = Slice{Auto, 3, step}.indices(8);
      CHECK(std::get<0>(second2) == 0);
      CHECK(std::get<1>(second2) == 3);
      CHECK(std::get<2>(second2) == step);

      idcs_t first = Slice{8, Auto, step}.indices(15);
      CHECK(std::get<0>(first) == 8);
      CHECK(std::get<1>(first) == 15);
      CHECK(std::get<2>(first) == step);
    }

    idcs_t one1 = Slice{8}.indices(15);
    CHECK(std::get<0>(one1) == 8);
    CHECK(std::get<1>(one1) == 9);
    CHECK(std::get<2>(one1) == 1);

    idcs_t one2 = Slice{2}.indices(20);
    CHECK(std::get<0>(one2) == 2);
    CHECK(std::get<1>(one2) == 3);
    CHECK(std::get<2>(one2) == 1);
  }

  SECTION("Check computed indices for negative step") {
    for (ptrdiff_t step = -1; step > -5; --step) {
      idcs_t second1 = Slice{Auto, 8, step}.indices(13);
      CHECK(std::get<0>(second1) == 12);
      CHECK(std::get<1>(second1) == 8);
      CHECK(std::get<2>(second1) == step);

      idcs_t second2 = Slice{Auto, 3, step}.indices(8);
      CHECK(std::get<0>(second2) == 7);
      CHECK(std::get<1>(second2) == 3);
      CHECK(std::get<2>(second2) == step);

      idcs_t first1 = Slice{8, Auto, step}.indices(15);
      CHECK(std::get<0>(first1) == 8);
      CHECK(std::get<1>(first1) == -1);
      CHECK(std::get<2>(first1) == step);

      idcs_t first2 = Slice{10, Auto, step}.indices(20);
      CHECK(std::get<0>(first2) == 10);
      CHECK(std::get<1>(first2) == -1);
      CHECK(std::get<2>(first2) == step);
    }
  }

#ifndef NDEBUG
  SECTION("Empty ranges throw errors") {
    using Catch::Matchers::Contains;

    CHECK_THROWS_AS(Slice(4, 4).indices(10), IndexError);
    CHECK_THROWS_AS(Slice(6, 4).indices(10), IndexError);
    CHECK_THROWS_WITH(Slice(6, 6, -1).indices(10), Contains("is empty"));
    CHECK_THROWS_WITH(Slice(4, 6, -1).indices(10), Contains("is empty"));
  }

  SECTION("Invalid ranges throw errors") {
    using Catch::Matchers::Contains;

    CHECK_THROWS_AS(Slice(10).indices(10), IndexError);
    CHECK_NOTHROW(Slice(10).indices(11));

    CHECK_THROWS_WITH(Slice(2, 11).indices(10), Contains("beyond the valid index range"));
    CHECK_NOTHROW(Slice(2, 10).indices(10));

    CHECK_THROWS_WITH(Slice(4, 0, -1).indices(4),
                      Contains("beyond the valid index range"));
    CHECK_NOTHROW(Slice(4, 0, -1).indices(5));

    CHECK_THROWS_WITH(Slice(10, 20, 1).indices(10),
                      Contains("beyond the valid index range"));
    CHECK_THROWS_WITH(Slice(20, 9, -1).indices(10),
                      Contains("beyond the valid index range"));
  }
#endif  // NDEBUG
}

}  // namespace pammap
