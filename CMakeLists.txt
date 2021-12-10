cmake_minimum_required(VERSION 3.19)
project(eiv LANGUAGES CXX RC)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

find_package(eunomia REQUIRED)
find_package(urania REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)


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
  $<$<CXX_COMPILER_ID:Clang>:-municode>
  $<$<CXX_COMPILER_ID:GNU>:-municode>
)
target_link_libraries(eiv urania::urania eunomia::eunomia)
if (MSVC)
  target_compile_options(eiv PRIVATE /source-charset:utf-8)
endif()

target_compile_definitions(eiv PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)

target_link_options(eiv PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-static -municode>
  $<$<CXX_COMPILER_ID:GNU>:-static -municode>
)

#INSTALL(TARGETS eiv DESTINATION bin)


#eof
