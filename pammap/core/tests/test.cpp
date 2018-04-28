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

#include "core.hpp"
#include <catch2/catch.hpp>
#include <iostream>

namespace pammap {
namespace test {

TEST_CASE("Dummy test") {
  std::vector<Integer> list{1, 2, 3};
  ArrayView<Integer> view(list);

  PamMap map{
        {"a/b/c", 0},
        {"cde", 4},
        {"d/ou/ble", 2},
        {"test/list", view},
  };

  PamMap test = map.submap("test");
  test.update("list2", view);

  for (auto& kv : map) {
    if (kv.key() == "/test/list") {
      CHECK(kv.type_name() == "pammap::ArrayView<long>");
      CHECK(kv.value<ArrayView<Integer>>().shape()[0] == 3);
    } else if (kv.key() == "/test/list2") {
      CHECK(kv.type_name() == "pammap::ArrayView<long>");
      CHECK(kv.value<ArrayView<Integer>>().shape()[0] == 3);
    } else if (kv.key() == "/a/b/c") {
      CHECK(kv.type_name() == "long");
      CHECK(kv.value<Integer>() == 0);
    } else if (kv.key() == "/cde") {
      CHECK(kv.type_name() == "long");
      CHECK(kv.value<Integer>() == 4);
    } else if (kv.key() == "/d/ou/ble") {
      CHECK(kv.type_name() == "long");
      CHECK(kv.value<Integer>() == 2);
    }
  }

  PamMap sub = map.submap("a/b");
  REQUIRE(sub.at<Integer>("c") == 0);
}

}  // namespace test
}  // namespace pammap
