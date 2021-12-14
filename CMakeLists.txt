# CMakeLists.txt
#
# by oZ/acy
# Copyright 2021 oZ/acy (名賀月晃嗣) <acy@hiemalis.org>
#
# Redistribution and use in source and binary forms, 
#     with or without modification, 
#   are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.19)
project(eiv LANGUAGES CXX RC)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS
  "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)"
  OFF
)

# staticなライブラリをビルドする場合は、依存ライブラリもstaticにする。
if (NOT BUILD_SHARED_LIBS AND NOT MSVC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.dll.a;.lib)
endif()


# 依存するライブラリ
find_package(eunomia REQUIRED)
find_package(urania REQUIRED)
#find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)


#### eiv.exe ########
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
  $<$<BOOL:${MINGW}>:-municode>
  $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>
)
target_link_libraries(eiv urania::urania eunomia::eunomia)

target_compile_definitions(eiv PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x0A00)

target_link_options(eiv PRIVATE
  $<$<BOOL:${MINGW}>:-municode>
  $<$<CXX_COMPILER_ID:MSVC>:/NODEFAULTLIB:libcmt>
)

#INSTALL(TARGETS eiv DESTINATION ${CMAKE_INSTALL_BINDIR})


#eof
