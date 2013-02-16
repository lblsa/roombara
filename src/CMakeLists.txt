add_executable(roombara main.cpp)
target_link_libraries( roombara ${OpenCV_LIBS} )

add_executable(kalman kalman.cpp)
target_link_libraries( kalman ${OpenCV_LIBS} )

add_executable(main1 main1.cpp)
target_link_libraries( main1 ${OpenCV_LIBS} )

add_executable(filters filters.cpp)
target_link_libraries( filters ${OpenCV_LIBS} )

add_executable(morphology morphology.cpp)
target_link_libraries( morphology ${OpenCV_LIBS} )

add_executable(threshold threshold.cpp)
target_link_libraries( threshold ${OpenCV_LIBS} )

add_executable(algo algo.cpp)
target_link_libraries( algo ${OpenCV_LIBS} )

add_executable(haar haar.cpp)
target_link_libraries( haar ${OpenCV_LIBS} )

add_executable(findContours_demo findContours_demo.cpp)
target_link_libraries( findContours_demo ${OpenCV_LIBS} )

add_executable(33-cvMatchShapes 33-cvMatchShapes.cpp)
target_link_libraries( 33-cvMatchShapes ${OpenCV_LIBS} )

add_executable(lkdemo lkdemo.cpp)
target_link_libraries( lkdemo ${OpenCV_LIBS} )

add_executable(111 111.cpp)
target_link_libraries( 111 ${OpenCV_LIBS} )

add_executable(112 112.cpp)
target_link_libraries( 112 ${OpenCV_LIBS} )
