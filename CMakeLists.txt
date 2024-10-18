cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project("PhysY" VERSION 0.3.0)

add_executable(${PROJECT_NAME} "PhysY.cpp" "etc/color.cpp")
include_directories("etc/")

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED YES)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS NO)

##########
set(ARGS "-fpic -fprofile-generate -funroll-loops -fomit-frame-pointer -finline-functions -pedantic -Wall -Wextra -Wunused -Wdeprecated -Os -march=native -mtune=native")
set(CARGS "-fpeel-loops -fdce -fipa-cp -fipa-icf ${ARGS}")
##########

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "-malign-data -fprefetch-loops ${CARGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE ${CARGS})
else()
    set(CMAKE_CXX_FLAGS_RELEASE ${ARGS})
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
