# CMakeLists.txt for EIV

cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE 1)
project(eiv CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(eunomia REQUIRED)
find_package(urania REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)


# .rcの處理の定義
set(CMAKE_RC_COMPILER windres)
enable_language(RC)
set(CMAKE_RC_COMPILE_OBJECT
  "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")


# eiv.exe
add_executable(eiv WIN32
  main.cpp
  eiv.cpp
  eiv2.cpp
  eivwmh.cpp
  savepct.cpp
  towallpaper.cpp
  eiv_res.rc
#  util.cpp
)

target_compile_options(eiv PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3 -municode>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3 -municode>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

target_link_libraries(eiv  urania eunomia jpeg png z)

target_compile_definitions(eiv PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)

target_link_options(eiv PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static -municode>
  $<$<CXX_COMPILER_ID:GNU>:-static -municode>
)

#INSTALL(TARGETS eiv DESTINATION bin)


#eof
