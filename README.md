# cyclonedds_pub_sub_app
For building

mkdir build
cd build
cmake  -G "Visual Studio 15 2017 Win64"  -DCMAKE_PREFIX_PATH="C:\bplus_tools\BS_CycloneDDS\install_location_cyclone_dds;C:\bplus_tools\BS_CycloneDDS\install_location_cyclonedds_cxx"  C:\Projects\New_folder\cyclonedds_pub_sub_app
conan install #{conan_file} --build missing --profile=#{conan_profile} --no-imports
