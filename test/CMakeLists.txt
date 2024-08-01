cmake_minimum_required(VERSION 3.0.2)
project(test)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})
MESSAGE( "current dir ${CMAKE_CURRENT_SOURCE_DIR}")
MESSAGE( "STATUS ===========================================================")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g ")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# include(CheckCXXCompilerFlag)
# CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
# if(COMPILER_SUPPORTS_CXX17)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#    add_definitions(-DCOMPILEDWITHC17)
#    message(STATUS "Using flag -std=c++17.")
# else()
#    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
# endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(EIGEN3_INCLUDE_DIR "/usr/local/include/eigen3")
# find_package(Eigen3 REQUIRED)

set(OpenCV_Dir "/usr/local/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
message(STATUS "======= ${OpenCV_INCLUDE_DIRS}")
find_package(Boost  REQUIRED)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/GeographicLib-1.51/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/manif/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/manif/external/lt
)

message(STATUS "eigen dir ${EIGEN3_INCLUDE_DIR}")

file(GLOB_RECURSE  HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/pugixml/*.hpp 
                             ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
                             ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/GeographicLib-1.51/include/GeographicLib/*.hpp)
file(GLOB_RECURSE  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/pugixml/*.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/GeographicLib-1.51/src/*.cpp )
add_executable(code_test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp
                          ${HEAD_FILE}
                          ${SOURCE_FILES})
target_link_libraries(code_test ${OpenCV_LIBS} ${Boost_LIBRARIES} -pthread -fPIC )


