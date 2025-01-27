# CanFrame Publisher and Subscriber

This project demonstrates a publisher and subscriber implementation using `CanFrame` and `EthernetFrame` with C. It includes:

- CanFrame.idl - Interface Definition Language file.
- EthernetFrame.idl
- Header.idl
- Time.idl 
- Publisher and subscriber applications.

## Requirements

- CMake
- Visual Studio 2017 (Win64)
- CycloneDDS (version 0.10.2)
- Conan

## Build Instructions

1. Generate project files using CMake:
   ```bash
   mkdir build
   cd build
   cmake  -G "Visual Studio 15 2017 Win64"  -DCMAKE_PREFIX_PATH="C:\bplus_tools\BS_CycloneDDS\install_location_cyclone_dds;C:\bplus_tools\BS_CycloneDDS\install_location_cyclonedds_cxx"  C:\Projects\New_folder\cyclonedds_pub_sub_app
   conan install #{conan_file} --build missing --profile=#{conan_profile} --no-imports



