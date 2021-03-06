// This file is part of the Acts project.
//
// Copyright (C) 2017-2018 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <boost/test/unit_test.hpp>

#include "Acts/Geometry/GeometryID.hpp"

namespace Acts {
namespace Test {

BOOST_AUTO_TEST_CASE(GeometryID_construct_default) {
  GeometryID id;
  BOOST_TEST(id.volume() == 0u);
  BOOST_TEST(id.boundary() == 0u);
  BOOST_TEST(id.layer() == 0u);
  BOOST_TEST(id.approach() == 0u);
  BOOST_TEST(id.sensitive() == 0u);
}

BOOST_AUTO_TEST_CASE(GeometryID_construct_encoded) {
  // not sure if it is a good idea to test for the encoding since it should be
  // an implementation detail. only the resulting functionality is relevant.
  GeometryID id = 0xa0b00c00d00affe0u;
  BOOST_TEST(id.volume() == 0xa0u);
  BOOST_TEST(id.boundary() == 0xb0u);
  BOOST_TEST(id.layer() == 0x0c0u);
  BOOST_TEST(id.approach() == 0x0du);
  BOOST_TEST(id.sensitive() == 0x00affe0u);
}

BOOST_AUTO_TEST_CASE(GeometryID_max_values) {
  // compute maximum value for each component
  constexpr GeometryID::Value volumeMax = (1u << 8) - 1;
  constexpr GeometryID::Value boundaryMax = (1u << 8) - 1;
  constexpr GeometryID::Value layerMax = (1u << 12) - 1;
  constexpr GeometryID::Value approachMax = (1u << 8) - 1;
  constexpr GeometryID::Value sensitiveMax = (1u << 28) - 1;
  // reference values non-zero values everywhere
  constexpr GeometryID ref = 0xdeadaffe01234567;
  // values above the maximum are truncated
  // max+1 has all available bits zeroed
  BOOST_TEST(GeometryID(ref).setVolume(volumeMax + 1) ==
             GeometryID(ref).setVolume(0u));
  BOOST_TEST(GeometryID(ref).setBoundary(boundaryMax + 1) ==
             GeometryID(ref).setBoundary(0u));
  BOOST_TEST(GeometryID(ref).setLayer(layerMax + 1) ==
             GeometryID(ref).setLayer(0u));
  BOOST_TEST(GeometryID(ref).setApproach(approachMax + 1) ==
             GeometryID(ref).setApproach(0u));
  BOOST_TEST(GeometryID(ref).setSensitive(sensitiveMax + 1) ==
             GeometryID(ref).setSensitive(0u));
}

BOOST_AUTO_TEST_CASE(GeometryID_order) {
  auto vol1 = GeometryID().setVolume(1u).setLayer(14u).setSensitive(5u);
  auto vol2 = GeometryID().setVolume(2u).setLayer(13u).setSensitive(3u);
  // order uses volume first even if other components are larger
  BOOST_TEST(vol1 < vol2);
  BOOST_TEST(GeometryID(vol1).setBoundary(64u) < vol2);
  BOOST_TEST(GeometryID(vol1).setLayer(64u) < vol2);
  BOOST_TEST(GeometryID(vol1).setApproach(64u) < vol2);
  BOOST_TEST(GeometryID(vol1).setSensitive(64u) < vol2);
  BOOST_TEST(vol2 < GeometryID(vol1).setVolume(3u));
  // other components are hierachical
  BOOST_TEST(GeometryID(vol1).setVolume(1u).setBoundary(2u) <
             GeometryID(vol1).setVolume(2u).setBoundary(1u));
  BOOST_TEST(GeometryID(vol1).setBoundary(1u).setLayer(2u) <
             GeometryID(vol1).setBoundary(2u).setLayer(1u));
  BOOST_TEST(GeometryID(vol1).setLayer(1u).setApproach(2u) <
             GeometryID(vol1).setLayer(2u).setApproach(1u));
  BOOST_TEST(GeometryID(vol1).setApproach(1u).setSensitive(2u) <
             GeometryID(vol1).setApproach(2u).setSensitive(1u));
}

}  // namespace Test
}  // namespace Acts
