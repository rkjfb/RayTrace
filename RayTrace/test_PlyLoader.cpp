#include "pch.h"

#include "PlyLoader.h"

using namespace ray;

//Feature: OBJ File Parser
//
//Scenario: Ignoring unrecognized lines
//  Given gibberish ← a file containing:
//    """
//    There was a young lady named Bright
//    who traveled much faster than light.
//    She set out one day
//    in a relative way,
//    and came back the previous night.
//    """
//  When parser ← parse_obj_file(gibberish)
//  Then parser should have ignored 5 lines
TEST(PlyLoader, Cube) {
	PlyLoader ply("cube.ply");

	auto g = ply.get_mega_group();
	EXPECT_EQ(g->bounds(), Bounds(Point3(), Point3(1, 1, 1)));
	auto vec = g->extract_shapes();
	EXPECT_EQ(vec.size(), 12);
}