// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#define BOOST_TEST_MODULE Ellipse Bounds Tests

#include <boost/test/included/unit_test.hpp>
// leave blank

#include <boost/test/data/test_case.hpp>
// leave blank

#include <boost/test/output_test_stream.hpp>
// leave blank

//
#include "ACTS/Surfaces/EllipseBounds.hpp"
#include "ACTS/Utilities/Definitions.hpp"
//
#include <limits>

// namespace bdata = boost::unit_test::data;
namespace utf    = boost::unit_test;
const double inf = std::numeric_limits<double>::infinity();
const double NaN = std::numeric_limits<double>::quiet_NaN();

namespace Acts {

namespace Test {
  BOOST_AUTO_TEST_SUITE(Surfaces);
  /// Unit test for creating compliant/non-compliant EllipseBounds object
  BOOST_AUTO_TEST_CASE(EllipseBoundsConstruction)
  {
    double minRad1(10.), minRad2(15.), maxRad1(15.), maxRad2(20.),
        averagePhi(0.), phiSector(M_PI / 2.);
    // test default construction
    // EllipseBounds defaultConstructedEllipseBounds;  //deleted
    //
    /// Test construction with dimensions
    BOOST_TEST(
        EllipseBounds(minRad1, minRad2, maxRad1, maxRad2, averagePhi, phiSector)
            .type()
        == SurfaceBounds::Ellipse);
    //
    /// Copy constructor
    EllipseBounds original(
        minRad1, minRad2, maxRad1, maxRad2, averagePhi, phiSector);
    EllipseBounds copied(original);
    BOOST_TEST(copied.type() == SurfaceBounds::Ellipse);
  }

  /// Unit tests for EllipseBounds properties
  BOOST_AUTO_TEST_CASE(EllipseBoundsProperties)
  {
    double minRad1(10.), minRad2(15.), maxRad1(15.), maxRad2(20.),
        averagePhi(0.), phiSector(M_PI / 2.);
    /// Test clone
    EllipseBounds ellipseBoundsObject(
        minRad1, minRad2, maxRad1, maxRad2, averagePhi, phiSector);
    auto pClonedEllipseBoundsObject = ellipseBoundsObject.clone();
    BOOST_TEST(bool(pClonedEllipseBoundsObject));
    delete pClonedEllipseBoundsObject;
    //
    /// Test type() (redundant; already used in constructor confirmation)
    BOOST_TEST(ellipseBoundsObject.type() == SurfaceBounds::Ellipse);
    //
    // clone already tested
    //
    /// Test distanceToBoundary
    Vector2D origin(0., 0.);
    Vector2D outsideBy10(0., 30.);
    Vector2D inRectangle(17., 11.);
    BOOST_TEST(ellipseBoundsObject.distanceToBoundary(origin)
               == 10.);  // makes sense
    BOOST_TEST(ellipseBoundsObject.distanceToBoundary(outsideBy10)
               == 10.);  // ok
    //
    /// Test rMinX
    BOOST_TEST(ellipseBoundsObject.rMinX() == minRad1);
    //
    /// Test rMinY
    BOOST_TEST(ellipseBoundsObject.rMinY() == minRad2);
    //
    /// Test rMaxX
    BOOST_TEST(ellipseBoundsObject.rMaxX() == maxRad1);
    //
    /// Test rMaxY
    BOOST_TEST(ellipseBoundsObject.rMaxY() == maxRad2);
    //
    /// Test averagePhi
    BOOST_TEST(ellipseBoundsObject.averagePhi() == averagePhi);
    //
    /// Test vertices
    std::vector<Vector2D> expectedVertices{
        {15, 0}, {0, 20}, {-15, 0}, {0, -20}};
    BOOST_TEST(ellipseBoundsObject.vertices() == expectedVertices);
    //
    /// Test boundingBox
    BOOST_TEST(ellipseBoundsObject.boundingBox() == RectangleBounds(15., 20.));
    //
    /// Test halfPhiSector
    BOOST_TEST(ellipseBoundsObject.halfPhiSector() == M_PI / 2.);
    //
    /// Test dump
    boost::test_tools::output_test_stream dumpOuput;
    ellipseBoundsObject.dump(dumpOuput);
    BOOST_TEST(dumpOuput.is_equal(
        "Acts::EllipseBounds:  (innerRadiusX, innerRadiusY, outerRadiusX, "
        "outerRadiusY, hPhiSector) = (10.0000000, 15.0000000, 15.0000000, "
        "20.0000000, 0.0000000, 1.5707963)"));
    //
    /// Test inside
    BOOST_TEST(ellipseBoundsObject.inside(inRectangle, BoundaryCheck(true))
               == false);
    // dont understand why this is so:
    BOOST_TEST(ellipseBoundsObject.inside(outsideBy10, BoundaryCheck(true))
               == false);
    //
    /// Test insideLoc0 (only checks for inside bounding rectangle...don't
    /// understand answer here)
    BOOST_TEST(ellipseBoundsObject.insideLoc0(inRectangle) == false);
    //
    /// Test insideLoc1 (only checks for inside bounding rectangle)
    BOOST_TEST(ellipseBoundsObject.insideLoc1(inRectangle) == true);
    //
  }
  /// Unit test for testing EllipseBounds assignment
  BOOST_AUTO_TEST_CASE(EllipseBoundsAssignment)
  {
    double minRad1(10.), minRad2(15.), maxRad1(15.), maxRad2(20.),
        averagePhi(0.), phiSector(M_PI / 2.);
    EllipseBounds ellipseBoundsObject(
        minRad1, minRad2, maxRad1, maxRad2, averagePhi, phiSector);
    EllipseBounds similarlyConstructeEllipseBoundsObject(
        minRad1, minRad2, maxRad1, maxRad2, averagePhi, phiSector);
    /// Test operator ==
    BOOST_TEST(ellipseBoundsObject == similarlyConstructeEllipseBoundsObject);
    //
    /// Test assignment
    EllipseBounds assignedEllipseBoundsObject(
        NaN, NaN, NaN, NaN, NaN);  // invalid
    // object, in some sense
    assignedEllipseBoundsObject = ellipseBoundsObject;
    BOOST_TEST(assignedEllipseBoundsObject == ellipseBoundsObject);
  }
  BOOST_AUTO_TEST_SUITE_END();

}  // end of namespace Test

}  // end of namespace Acts