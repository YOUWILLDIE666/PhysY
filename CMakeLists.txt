cmake_minimum_required (VERSION 3.8)
project ("PhysY" VERSION 0.3.0)

add_executable (${PROJECT_NAME} "PhysY-0.3.0D.cpp" "etc/color.cpp")
include_directories ("etc/")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

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

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
else()
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
endif()
