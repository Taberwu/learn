cmake_minimum_required(VERSION 3.14)
project(learn)

SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath=$ORIGIN")


add_subdirectory(test)
#add_subdirectory(imu_align)
# add_subdirectory(sp_dr)



