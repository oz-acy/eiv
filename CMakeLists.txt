# CMakeLists.txt for EIV

cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE 1)
project(eiv CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(themis REQUIRED)
find_package(polymnia REQUIRED)
find_package(urania REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)


#set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -O3 -DWINVER=0x500 -D_UNICODE -DUNICODE -municode")
#SET(CMAKE_LD_FLAGS "-mwindows -static")


#include_directories("${CMAKE_INSTALL_PREFIX}/include")
#LINK_DIRECTORIES("${CMAKE_INSTALL_PREFIX}/lib")

# .rcの處理の定義
set(CMAKE_RC_COMPILER windres)
enable_language(RC)
set(CMAKE_RC_COMPILE_OBJECT
  "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")


# eiv.exe
add_executable(eiv
  main.cpp
  eiv.cpp
  eiv2.cpp
  eivwmh.cpp
  savepct.cpp
  towallpaper.cpp
  eiv_res.rc
#  util.cpp
)

#SET_TARGET_PROPERTIES(eiv PROPERTIES LINK_FLAGS "-mwindows -static -municode")

target_compile_options(eiv PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3 -municode>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3 -municode>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

target_link_libraries(eiv
#  urania polymnia themis jpeg png z boost_filesystem-mt boost_system-mt stdc++fs)
#  urania polymnia themis jpeg png z stdc++fs)
  urania polymnia themis jpeg png z)

target_compile_definitions(eiv PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)

target_link_options(eiv PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-mwindows -static -municode>
  $<$<CXX_COMPILER_ID:GNU>:-mwindows -static -municode>
)





#INSTALL(TARGETS eiv DESTINATION bin)


#eof
