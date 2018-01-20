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

#include <catch.hpp>
#include <iostream>
#include <pammap/pammap.hpp>

namespace pammap {
namespace test {

TEST_CASE("Dummy test") {
  PamMap map{
        {"a/b/c", 0},
        {"cde", 4},
        {"d/ou/ble", 2},
        {"test/list", {1, 2, 3}},
  };

  PamMap test = map.submap("test");
  test.update("list2", DataBlock<Integer>{1, 2, 3});

  for (auto& kv : map) {
    if (kv.key() == "/test/list" || kv.key() == "/test/list2") {
      std::cout << kv.key() << " " << kv.type_name() << " = "
                << kv.value<DataBlock<Integer>>().size() << std::endl;
    } else {
      std::cout << kv.key() << " " << kv.type_name() << " = " << kv.value<Integer>()
                << std::endl;
    }
  }

  PamMap sub = map.submap("a/b");
  REQUIRE(sub.at<Integer>("c") == 0);
}

}  // namespace test
}  // namespace pammap
