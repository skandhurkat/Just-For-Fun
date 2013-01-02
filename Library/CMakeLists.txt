cmake_minimum_required (VERSION 2.6)
project (Library)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR
      "This code compiles only on Windows")
endif()

add_executable (Library src/msConGra.h src/msConGra.cpp src/KeyMaker.h src/KeyMaker.cpp src/Library.h src/Library.cpp src/LibApp.cpp)
add_executable (KeyMaker src/msConGra.h src/msConGra.cpp src/KeyMaker.h src/KeyMaker_MakeKey.cpp src/MakeKey.cpp)
install (TARGETS Library KeyMaker DESTINATION bin)
install (FILES src/Application.sif src/Books.dat src/Members.dat DESTINATION bin)
