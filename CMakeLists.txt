cmake_minimum_required(VERSION 3.4.1)

project(lda)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")#-Wl -rpath /home/wudi/Software/android-ndk-r21e-linux-x86_64/android-ndk-r21e/platforms/android-30/arch-arm64/usr/lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")#-Wl,--allow-shlib-undefined  



include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/cppjieba/include)
add_executable(lda src/lda.cpp src/model.cpp src/dataset.cpp src/utils.cpp)
find_package(GSL REQUIRED)
target_link_libraries(lda GSL::gsl GSL::gslcblas)




