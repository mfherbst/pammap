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

#include <iostream>
#include <pammap/pammap.hpp>

namespace pammap {
namespace test {

int main() {
  PamMap map{{"a/b/c", 0}, {"cde", 4}, {"d/ou/ble", 2}};

  for (auto& kv : map) {
    std::cout << kv.key() << " " << kv.type_name() << " = " << kv.value<int>()
              << std::endl;
  }

  auto sub = map.submap("a/b");
  return sub.at<int>("c");
}

}  // namespace test
}  // namespace pammap

int main() { return pammap::test::main(); }
