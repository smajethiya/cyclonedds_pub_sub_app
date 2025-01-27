cmake_minimum_required(VERSION 3.16)
project(CanFrameProject LANGUAGES C CXX)

if(NOT TARGET CycloneDDS::ddsc)
  find_package(CycloneDDS REQUIRED)
endif()

# Specify IDL files
set(CANFRAME_IDL_FILES
    ${CMAKE_SOURCE_DIR}/idl/CanFrame.idl
    ${CMAKE_SOURCE_DIR}/idl/Header.idl
    ${CMAKE_SOURCE_DIR}/idl/Time.idl
)

set(ETHERNETFRAME_IDL_FILE
    ${CMAKE_SOURCE_DIR}/idl/EthernetFrame.idl
)

# Specify source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/CanFrame_pub.c
    ${CMAKE_SOURCE_DIR}/src/CanFrame_sub.c
    ${CMAKE_SOURCE_DIR}/src/EthernetFrame_pub.c
    ${CMAKE_SOURCE_DIR}/src/EthernetFrame_sub.c
    ${CMAKE_SOURCE_DIR}/src/header_pub.c
    ${CMAKE_SOURCE_DIR}/src/header_sub.c
    ${CMAKE_SOURCE_DIR}/src/timemsg_pub.c
    ${CMAKE_SOURCE_DIR}/src/timemsg_sub.c
)

# Generate code from IDL files for CanFrame and related files
idlc_generate(
    TARGET can_data
    FILES ${CANFRAME_IDL_FILES}
    WARNINGS no-implicit-extensibility
)

# Generate code from IDL files for EthernetFrame
idlc_generate(
    TARGET ethernet_data
    FILES ${ETHERNETFRAME_IDL_FILE}
    WARNINGS no-implicit-extensibility
)

# Define executables for CanFrame
add_executable(ddsc_canframe_pub ${CMAKE_SOURCE_DIR}/src/CanFrame_pub.c)
add_executable(ddsc_canframe_sub ${CMAKE_SOURCE_DIR}/src/CanFrame_sub.c)

# Define executables for EthernetFrame
add_executable(ddsc_ethernetframe_pub ${CMAKE_SOURCE_DIR}/src/EthernetFrame_pub.c)
add_executable(ddsc_ethernetframe_sub ${CMAKE_SOURCE_DIR}/src/EthernetFrame_sub.c)

# Link libraries
target_link_libraries(ddsc_canframe_pub CycloneDDS::ddsc can_data)
target_link_libraries(ddsc_canframe_sub CycloneDDS::ddsc can_data)
target_link_libraries(ddsc_ethernetframe_pub CycloneDDS::ddsc ethernet_data)
target_link_libraries(ddsc_ethernetframe_sub CycloneDDS::ddsc ethernet_data)

# Optional: Add static library for shared sources (if needed)
add_library(common_frames STATIC ${SOURCES})

# General setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

conan_basic_setup(TARGETS)

