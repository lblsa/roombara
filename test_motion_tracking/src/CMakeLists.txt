add_executable(test_motion_tracking main.cpp)
include_directories("/usr/include/opencv")
target_link_libraries( test_motion_tracking ${OpenCV_LIBS} )
