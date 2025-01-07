cmake_minimum_required(VERSION 3.16)
project(CanFrameProject LANGUAGES C CXX)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

 set(SOURCES
    canframe_pub.cpp
    canframe_sub.cpp
 )
 
  add_library(canframe STATIC ${SOURCES})

idlcxx_generate(TARGET can_data FILES CanFrame.idl WARNINGS no-implicit-extensibility)

 add_executable(ddscxx_canframe_pub canframe_pub.cpp)
 add_executable(ddscxx_canframe_sub canframe_sub.cpp)

 target_link_libraries(ddscxx_canframe_pub CycloneDDS-CXX::ddscxx can_data)
 target_link_libraries(ddscxx_canframe_sub CycloneDDS-CXX::ddscxx can_data)

 target_include_directories(can_data INTERFACE C:/sw_lib_dds_messages/sw_lib_dds_messages/definitions)