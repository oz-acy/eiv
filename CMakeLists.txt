# CMakeLists.txt for EIV

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
SET(CMAKE_VERBOSE_MAKEFILE 1)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -O3 -DWINVER=0x500 -D_UNICODE -DUNICODE -municode")
#SET(CMAKE_LD_FLAGS "-mwindows -static")

#PROJECT(eiv CXX)

include_directories("${CMAKE_INSTALL_PREFIX}/include")
LINK_DIRECTORIES("${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_RC_COMPILER windres)
enable_language(RC)
SET(CMAKE_RC_COMPILE_OBJECT
  "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")

ADD_EXECUTABLE(eiv
  main.cpp
  eiv.cpp
  eiv2.cpp
  eiv_res.rc
  eivwmh.cpp
  savepct.cpp
  towallpaper.cpp
#  util.cpp
)

SET_TARGET_PROPERTIES(eiv PROPERTIES LINK_FLAGS "-mwindows -static -municode")
#SET_TARGET_PROPERTIES(eiv PROPERTIES LINK_FLAGS "-static -municode")

TARGET_LINK_LIBRARIES(eiv
#  urania polymnia themis jpeg png z boost_filesystem-mt boost_system-mt stdc++fs)
  urania polymnia themis jpeg png z stdc++fs)
INSTALL(TARGETS eiv DESTINATION bin)


#eof
