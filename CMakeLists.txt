cmake_minimum_required (VERSION 2.6)
project (roombara)
find_package( OpenCV REQUIRED )

include_directories (include)

add_definitions(-ggdb)

add_subdirectory (src)
add_subdirectory (test_motion_tracking/src)
