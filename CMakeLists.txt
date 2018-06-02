# 2.8.8 required to use PROTOBUF_IMPORT_DIRS
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
FIND_PACKAGE( Boost COMPONENTS system filesystem REQUIRED )
FIND_PACKAGE( OpenCV REQUIRED )
set (CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  pkg_check_modules(SDF sdformat)
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${SDF_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/msgs
)

include_directories(include)
# Spawner gazebo server plugin
add_library(pattern_generation SHARED src/PatternGeneration.cpp src/PerlinNoise.cpp)
target_link_libraries(
    pattern_generation
    ${Boost_LIBRARIES} ${OpenCV_LIBS})


# Spawner client
add_executable (
    pattern_generation_test
    src/tests/pattern_generation_test.cpp
)

target_link_libraries(
    pattern_generation_test
    pattern_generation
    ${Boost_LIBRARIES} ${OpenCV_LIBS}
)
