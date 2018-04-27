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
#include "demangle.hpp"
#include "exceptions.hpp"
#include <catch2/catch.hpp>

namespace pammap {
namespace tests {

TEST_CASE("PamMap", "[pammap]") {
  // Some data:
  Integer i = 5;
  String s{"test"};
  Float f = 7.5;

  // Some vectors to point into
  std::vector<Float> fvec{1., 2., 3., 4.};
  std::vector<Integer> ivec{1, 2, 3, 4};
  ArrayView<Float> farr(fvec);
  ArrayView<Integer> iarr(ivec);

  //
  // ---------------------------------------------------------------
  //

  // The expected demangled typenames for Integer and Float
  const std::string integer_typename = "long";
  const std::string float_typename   = "double";

  SECTION("Check typenames demangle as expected") {
    // if not this indicates that some tests might fail here
    // even though this is not the fault of the PamMap,
    // but of the type demangling.
    //
    //
    CHECK(integer_typename == demangle(typeid(Integer)));
    CHECK(float_typename == demangle(typeid(Float)));
  }

  //
  // ---------------------------------------------------------------
  //

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

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check for KeyError") {
    PamMap m{};
    m.update("i", i);

    REQUIRE_THROWS_AS(m.at<Bool>("blubber"), KeyError);
    REQUIRE_THROWS_AS(m.at<Integer>("blubb"), KeyError);
    REQUIRE(m.at<Integer>("i") == i);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Test update function") {
    PamMap m;
    Float d = 3.4;
    m.update("double", d);
    m.update("noref", 3.141592);
    m.update("word", "some");
    m.update("bool", true);
    m.update("not bool", false);
    m.update("farr", farr);
    m.update("iarr", iarr);

    REQUIRE(m.at<Float>("double") == d);
    REQUIRE(m.at<Float>("noref") == 3.141592);
    REQUIRE(m.at<Bool>("bool"));
    REQUIRE_FALSE(m.at<Bool>("not bool"));
    REQUIRE(m.at<String>("word") == "some");
    REQUIRE(m.at<ArrayView<Float>>("farr") == farr);
    REQUIRE(m.at<ArrayView<Integer>>("iarr") == iarr);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Test retrieving data from const maps") {
    PamMap m;
    m.update("double", 1.24);
    m.update("noref", 3.141592);
    m.update("word", "some");
    m.update("farr", farr);

    const PamMap& mref{m};
    REQUIRE(mref.at<Float>("double") == 1.24);
    REQUIRE(mref.at<Float>("noref") == 3.141592);
    REQUIRE(mref.at<String>("word") == "some");
    REQUIRE(mref.at<ArrayView<Float>>("farr") == farr);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Test at with default return") {
    std::vector<Float> other{6., 5., 4., 2.};
    ArrayView<Float> anderes(other);

    PamMap m;
    m.update("string", s);
    m.update("integer", i);
    m.update("farr", farr);

    REQUIRE(m.at<Integer>("blubber", 4) == 4);
    REQUIRE(m.at<String>("blub", "neun") == "neun");
    REQUIRE(m.at("string", String("neun")) == s);
    REQUIRE(m.at("123", farr) == farr);
    REQUIRE(m.at("farr", anderes) == farr);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Test construction from initialiser list") {
    PamMap m{{"value1", 1}, {"word", "a"}, {"integer", i}, {"string", s}, {"farr", farr}};

    REQUIRE(m.at<Integer>("value1") == 1);
    REQUIRE(m.at<String>("word") == "a");
    REQUIRE(m.at<Integer>("integer") == i);
    REQUIRE(m.at<String>("string") == s);
    REQUIRE(m.at<ArrayView<Float>>("farr") == farr);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Test insert_default") {
    const PamMap m{{"double", 3.4}, {"pi", 3.141592}};
    REQUIRE(m.at<Float>("double") == 3.4);
    REQUIRE(m.at<Float>("pi") == 3.141592);

    m.insert_default("string", "blubba");
    m.insert_default({{"one", 1}, {"two", 2}});

    REQUIRE(m.at<String>("string") == "blubba");
    REQUIRE(m.at<Integer>("one") == 1);
    REQUIRE(m.at<Integer>("two") == 2);
    REQUIRE(m.at<Float>("double") == 3.4);
    REQUIRE(m.at<Float>("pi") == 3.141592);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check basic path transformations") {
    // Add data to map.
    PamMap m{};
    m.update("one/two/three", "3");
    m.update("three/two/one", 4);
    m.update("", "test");

    // check it is there:
    REQUIRE(m.at<String>("one/two/three") == "3");
    REQUIRE(m.at<Integer>("three/two/one") == 4);
    REQUIRE(m.at<String>("") == "test");

    // Check the path normalisation:
    REQUIRE(m.at<String>("/one/two/three") == "3");
    REQUIRE(m.at<String>("/one/two//three") == "3");
    REQUIRE(m.at<Integer>("three/./two/one") == 4);
    REQUIRE(m.at<Integer>("three/./two/./one") == 4);
    REQUIRE(m.at<String>("one/two/three/") == "3");
    REQUIRE(m.at<Integer>("three/two/one/.") == 4);
    REQUIRE(m.at<String>("/one/./two/////three") == "3");
    REQUIRE(m.at<Integer>("/././/three///two/./one/") == 4);
    REQUIRE(m.at<Integer>("three/two/../two/one") == 4);
    REQUIRE(m.at<Integer>("../../../three/two/one") == 4);
    REQUIRE(m.at<Integer>("/../../../one/../three/two/one") == 4);

    REQUIRE(m.at<String>("/") == "test");
    REQUIRE(m.at<String>(".") == "test");
    REQUIRE(m.at<String>("/../") == "test");
    REQUIRE(m.at<String>("/../.") == "test");
    REQUIRE(m.at<String>("/.././") == "test");
    REQUIRE(m.at<String>(".././") == "test");
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check that data can be erased") {
    PamMap m{};

    REQUIRE_FALSE(m.exists("bla"));

    // Add data to map.
    m.update("s", s);
    m.update("i", i);
    m.update("farr", farr);

    // check it is there:
    REQUIRE(m.exists("i"));
    REQUIRE(m.exists("s"));
    REQUIRE(m.exists("farr"));
    REQUIRE_FALSE(m.exists("bla"));

    // remove a few:
    m.erase("/i/././");
    m.erase("farr");

    // check they are there (or not)
    REQUIRE_FALSE(m.exists("i"));
    REQUIRE(m.exists("s"));
    REQUIRE_FALSE(m.exists("farr"));

    // Add more data to map
    m.update({{"tree/sub", s},
              {"tree/i", i},
              {"farr", farr},
              {"tree/value", 9},
              {"tree", "root"},
              {"/", "god"}});
    REQUIRE(m.exists("tree/sub"));
    REQUIRE(m.exists("tree/i"));
    REQUIRE(m.exists("farr"));
    REQUIRE(m.exists("tree/value"));
    REQUIRE(m.exists("tree"));
    REQUIRE(m.exists("/"));
    REQUIRE(m.exists("s"));

    // Erase second tree element.
    auto it = m.submap("tree").begin();
    ++it;
    m.erase(it);
    REQUIRE(m.exists("tree/sub"));
    REQUIRE(m.exists("farr"));
    REQUIRE(m.exists("tree/value"));
    REQUIRE(m.exists("tree"));
    REQUIRE(m.exists("/"));
    REQUIRE(m.exists("s"));
    REQUIRE_FALSE(m.exists("tree/i"));

    // Erase recursive
    m.erase_recursive("tree");
    REQUIRE(m.exists("/"));
    REQUIRE(m.exists("s"));
    REQUIRE(m.exists("farr"));
    REQUIRE_FALSE(m.exists("tree/sub"));
    REQUIRE_FALSE(m.exists("tree/i"));
    REQUIRE_FALSE(m.exists("tree/value"));
    REQUIRE_FALSE(m.exists("tree"));

    auto key = m.begin()->key();
    m.erase(m.begin());
    REQUIRE_FALSE(m.exists(key));
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check submap functionality") {
    // Add data to map.
    PamMap m{{"tree/sub", s},   {"tree/i", i},    {"farr", farr},
             {"tree/value", 9}, {"tree", "root"}, {"/", "god"}};

    // check it is there:
    REQUIRE(m.exists("tree/sub"));
    REQUIRE(m.exists("tree/i"));
    REQUIRE(m.exists("tree"));
    REQUIRE(m.exists("tree/value"));
    REQUIRE(m.exists("farr"));
    REQUIRE(m.exists("/"));

    PamMap sub = m.submap("tree");

    // Check existence:
    REQUIRE_FALSE(sub.exists("tree/sub"));
    REQUIRE_FALSE(sub.exists("tree/i"));
    REQUIRE_FALSE(sub.exists("farr"));
    REQUIRE(sub.exists("sub"));
    REQUIRE(sub.exists("i"));
    REQUIRE(sub.exists("value"));
    REQUIRE(sub.exists("/"));

    // Check value is appropriate:
    REQUIRE(sub.at<String>("sub") == s);
    REQUIRE(sub.at<Integer>("i") == i);
    REQUIRE(sub.at<Integer>("value") == 9);
    REQUIRE(sub.at<String>("/") == "root");
    REQUIRE(sub.at<String>("..") == "root");
    REQUIRE(sub.at<String>("../..") == "root");
    REQUIRE(sub.at<String>("/../.") == "root");
    REQUIRE(sub.at<String>(".././") == "root");
    REQUIRE(m.at<String>("/") == "god");

    // Check adding a new value in the submap:
    sub.update("neu", 1.23);
    sub.update("value", 10);
    REQUIRE(m.at<Float>("tree/neu") == 1.23);
    REQUIRE(m.at<Integer>("tree/value") == 10);

    // Check path normalisation for submap:
    PamMap sub2 = m.submap("/./tree/.");
    REQUIRE(sub2.at<String>("sub") == s);
    REQUIRE(sub2.at<Integer>("i") == i);
    REQUIRE(sub2.at<Integer>("value") == 10);

    // Check that clearing a subtree leaves the rest intact
    sub.clear();
    REQUIRE(m.at<String>("/") == "god");
    REQUIRE(m.exists("farr"));
    REQUIRE(m.exists("/"));
    REQUIRE_FALSE(m.exists("neu"));
    REQUIRE_FALSE(m.exists("tree/neu"));
    REQUIRE_FALSE(m.exists("tree"));
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check iterators with begin() and end()") {
    // Add data to map.
    PamMap m{{"tree/sub", s},  {"tree/i", i}, {"farr", farr},  {"tree/value", 9},
             {"tree", "root"}, {"/", "god"},  {"/zzz", "end"}, {"/zz", "mend"}};

    // Check we get all keys for starters:
    std::vector<String> ref{"/",         "/farr",       "/tree", "/tree/i",
                            "/tree/sub", "/tree/value", "/zz",   "/zzz"};
    auto itref = std::begin(ref);
    for (auto it = m.begin(); it != m.end(); ++it, ++itref) {
      REQUIRE(itref != std::end(ref));
      CHECK(*itref == it->key());
    }
    CHECK(itref == std::end(ref));

    // For const iterator:
    itref = std::begin(ref);
    for (auto it = m.cbegin(); it != m.cend(); ++it, ++itref) {
      REQUIRE(itref != std::end(ref));
      CHECK(*itref == it->key());
    }
    CHECK(itref == std::end(ref));

    // Get a submap:
    PamMap sub = m.submap("tree");

    // Check we get all keys of the submap:
    std::vector<String> subref{"/", "/i", "/sub", "/value"};
    auto itsubref = std::begin(subref);
    for (auto it = sub.begin(); it != sub.end(); ++it, ++itsubref) {
      REQUIRE(itsubref != std::end(subref));
      CHECK(*itsubref == it->key());
    }
    CHECK(itsubref == std::end(subref));

    // For const iterator:
    itsubref = std::begin(subref);
    for (auto it = sub.cbegin(); it != sub.cend(); ++it, ++itsubref) {
      REQUIRE(itsubref != std::end(subref));
      CHECK(*itsubref == it->key());
    }
    CHECK(itsubref == std::end(subref));

    // For iterator over subpath
    itsubref = std::begin(subref);
    for (auto it = m.begin("tree"); it != m.end("tree"); ++it, ++itsubref) {
      REQUIRE(itsubref != std::end(subref));
      CHECK(*itsubref == it->key());
    }
    CHECK(itsubref == std::end(subref));

    // For const iterator over subpath
    itsubref = std::begin(subref);
    for (auto it = m.cbegin("tree"); it != m.cend("tree"); ++it, ++itsubref) {
      REQUIRE(itsubref != std::end(subref));
      CHECK(*itsubref == it->key());
    }
    CHECK(itsubref == std::end(subref));

    // Iterator over empty range:
    for (auto& kv : m.submap("blubba")) {
      REQUIRE(false);  // We should never get here, since range empty
    }
    for (auto it = m.begin("blubba"); it != m.end("blubba"); ++it) {
      REQUIRE(false);  // We should never get here, since range empty
    }
    for (auto it = m.cbegin("blubba"); it != m.cend("blubba"); ++it) {
      REQUIRE(false);  // We should never get here, since range empty
    }
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check accessor interface of the iterator") {
    const Float pi = 3.14159265;
    PamMap map;
    const PamMap& cmap(map);

    // Add a bunch of ints and  doubles
    for (int i = 0; i < 9; ++i) {
      map.update("ints/" + std::to_string(i), i);
      map.update("doubles/pip" + std::to_string(i), pi * i);
    }

    // Check iteration over ints
    int iref = 0;
    for (auto& kv : map.submap("ints")) {
      CHECK(kv.key() == "/" + std::to_string(iref));
      CHECK(kv.value<Integer>() == iref);
      CHECK(kv.type_name() == integer_typename);
      ++iref;
    }

    // Check const iteration over ints
    iref = 0;
    for (auto& kv : cmap.submap("ints")) {
      CHECK(kv.key() == "/" + std::to_string(iref));
      CHECK(kv.value<Integer>() == iref);
      CHECK(kv.type_name() == integer_typename);
      ++iref;
    }

    // Iteration over doubles
    iref = 0;
    for (auto& kv : map.submap("doubles")) {
      CHECK(kv.key() == "/pip" + std::to_string(iref));
      CHECK(kv.value<Float>() == pi * iref);
      CHECK(kv.type_name() == float_typename);
      ++iref;
    }

    // Const iteration over doubles
    iref = 0;
    for (auto& kv : cmap.submap("doubles")) {
      CHECK(kv.key() == "/pip" + std::to_string(iref));
      CHECK(kv.value<Float>() == pi * iref);
      CHECK(kv.type_name() == float_typename);
      ++iref;
    }
  }  // Check accessor interface of the iterator

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check that copying submaps works.") {
    // Add data to map.
    PamMap m{{"tree/sub", s},   {"tree/i", i},    {"farr", farr},
             {"tree/value", 9}, {"tree", "root"}, {"/", "god"}};

    // Get a submap:
    const PamMap sub = m.submap("/tree");

    // Copy it:
    PamMap copy = sub;

    // Check existence:
    REQUIRE_FALSE(copy.exists("tree/sub"));
    REQUIRE_FALSE(copy.exists("tree/i"));
    REQUIRE_FALSE(copy.exists("farr"));
    REQUIRE(copy.exists("sub"));
    REQUIRE(copy.exists("i"));
    REQUIRE(copy.exists("value"));
    REQUIRE(copy.exists("/"));

    // Check value is appropriate:
    REQUIRE(copy.at<String>("sub") == s);
    REQUIRE(copy.at<Integer>("i") == i);
    REQUIRE(copy.at<Integer>("value") == 9);
    REQUIRE(copy.at<String>("/") == "root");
    REQUIRE(copy.at<String>("..") == "root");
    REQUIRE(copy.at<String>("../..") == "root");
    REQUIRE(copy.at<String>("/../.") == "root");
    REQUIRE(copy.at<String>(".././") == "root");
    REQUIRE(m.at<String>("/") == "god");

    // Check adding a new value in the copy does not
    // affect original.
    copy.update("neu", 1.23);
    copy.update("value", 10);
    REQUIRE_FALSE(m.exists("tree/neu"));
    REQUIRE(m.at<Integer>("tree/value") == 9);
    REQUIRE_FALSE(sub.exists("neu"));
    REQUIRE(sub.at<Integer>("value") == 9);
  }

  //
  // ---------------------------------------------------------------
  //

  SECTION("Check that updating from other maps works.") {
    // Add data to map.
    PamMap m{{"tree/sub", s},   {"tree/i", i},    {"farr", farr},
             {"tree/value", 9}, {"tree", "root"}, {"/", "god"}};

    // Make a second map
    PamMap n{{"house/window", true},
             {"house/open", "14-15"},
             {"garden", false},
             {"blubber", "blabla"},
             {"house/value", 14}};

    // Make submaps:
    PamMap subm = m.submap("tree");
    PamMap subn = n.submap("house");

    CHECK(subn.at<Bool>("window"));
    CHECK(subn.at<String>("open") == "14-15");
    CHECK(subn.at<Integer>("value") == 14);
    CHECK(subm.at<Integer>("i") == i);
    CHECK(subm.at<Integer>("value") == 9);
    CHECK_FALSE(subm.exists("window"));
    CHECK_FALSE(subm.exists("open"));

    subm.update(subn);

    CHECK(subm.at<Integer>("i") == i);
    CHECK(subm.at<Integer>("value") == 14);
    CHECK(subm.at<Bool>("window"));
    CHECK(subm.at<String>("open") == "14-15");
    CHECK(m.at<Integer>("tree/value") == 14);
    CHECK(m.at<String>("tree/open") == "14-15");
    CHECK(m.at<Bool>("tree/window"));

    m.update("tree/value", 42);
    CHECK(subm.at<Integer>("value") == 42);
    CHECK(subn.at<Integer>("value") == 14);
    CHECK(n.at<Integer>("house/value") == 14);

    std::vector<String> ref{"/",          "/farr",     "/tree",       "/tree/i",
                            "/tree/open", "/tree/sub", "/tree/value", "/tree/window"};
    auto itref = std::begin(ref);
    for (auto it = m.begin(); it != m.end(); ++it, ++itref) {
      REQUIRE(itref != std::end(ref));
      CHECK(*itref == it->key());
    }
    CHECK(itref == std::end(ref));

    std::vector<String> refn{"/blubber", "/garden", "/house/open", "/house/value",
                             "/house/window"};
    auto itrefn = std::begin(refn);
    for (auto it = n.begin(); it != n.end(); ++it, ++itrefn) {
      REQUIRE(itrefn != std::end(refn));
      CHECK(*itrefn == it->key());
    }
    CHECK(itrefn == std::end(refn));

    // Append map n to m:
    m.update("mapn", n);

    std::vector<String> refm{"/",
                             "/farr",
                             "/mapn/blubber",
                             "/mapn/garden",
                             "/mapn/house/open",
                             "/mapn/house/value",
                             "/mapn/house/window",
                             "/tree",
                             "/tree/i",
                             "/tree/open",
                             "/tree/sub",
                             "/tree/value",
                             "/tree/window"};

    auto itrefm = std::begin(refm);
    for (auto it = m.begin(); it != m.end(); ++it, ++itrefm) {
      REQUIRE(itrefm != std::end(refm));
      CHECK(*itrefm == it->key());
    }
    CHECK(itrefm == std::end(refm));

    CHECK(m.at<Integer>("mapn/house/value") == 14);
    CHECK(m.at<Bool>("mapn/house/window"));
    CHECK(m.at<String>("mapn/house/open") == "14-15");
  }

  //
  // ---------------------------------------------------------------
  //

  // TODO Test mass update from initialiser list

}  // TEST_CASE
}  // namespace tests
}  // namespace pammap
