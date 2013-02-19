add_library(roombara_matcher STATIC matcher_factory.cpp surf_matcher.cpp template_matcher.cpp)
target_link_libraries( roombara_matcher ${OpenCV_LIBS} )

add_executable(test_motion_tracking main.cpp)
include_directories("/usr/include/opencv", "../include")
target_link_libraries( test_motion_tracking ${OpenCV_LIBS} roombara_matcher)
