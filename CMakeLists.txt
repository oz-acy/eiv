# CMakeLists.txt for EIV

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
SET(CMAKE_VERBOSE_MAKEFILE 1)

set(CMAKE_C_COMPILER "/usr/bin/x86_64-w64-mingw32-clang")
set(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-clang++")
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -O3 -DWINVER=0x500 -D_UNICODE -DUNICODE")
#SET(CMAKE_LD_FLAGS "-mwindows -static")

PROJECT(EIV)

include_directories("${CMAKE_INSTALL_PREFIX}/include")
LINK_DIRECTORIES("${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_RC_COMPILER /usr/bin/x86_64-w64-mingw32-windres)
#SET(CMAKE_RC_COMPILE_OBJECT
#  "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")

ADD_EXECUTABLE(eiv
  main.cpp
  eiv.cpp
  eiv2.cpp
  eiv.rc
  eivwmh.cpp
  savepct.cpp
  towallpaper.cpp
#  util.cpp
)

SET_TARGET_PROPERTIES(eiv PROPERTIES LINK_FLAGS "-mwindows -static -municode")

TARGET_LINK_LIBRARIES(eiv
  urania polymnia themis jpeg png z boost_filesystem boost_system)
INSTALL(TARGETS eiv DESTINATION bin)




#eof
