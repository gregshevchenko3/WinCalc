# CMakeList.txt : CMake project for WinCalc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

FIND_PACKAGE( Boost)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
# Add source to this project's executable.
add_executable (WinCalc WIN32 "WinCalc.cpp" "WinCalc.h"  )
TARGET_LINK_LIBRARIES( WinCalc LINK_PUBLIC ${Boost_LIBRARIES} )


# TODO: Add tests and install targets if needed.
