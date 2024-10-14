# ----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(find-rezolution)
set(LIB_NAME findRezolution)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# ---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{ROOTSYS}/ect/cmake")
find_package(ROOT 6.30 REQUIRED COMPONENTS RIO Net RHTTP)
include(${ROOT_USE_FILE})


set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# Huuuuuge warning options!
add_compile_options(-O2 -fopenmp -pthread -Wall -Wextra -Wpedantic -Wshadow)

# ----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_library(${LIB_NAME} SHARED ${sources} ${headers})
target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES})
add_executable(${PROJECT_NAME} main.cpp ${headers})
target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
