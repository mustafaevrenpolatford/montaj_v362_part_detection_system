# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Define project name
project(montaj_v362_parca_kontrol_sistemi)

# Find OpenCV
# you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV 4 REQUIRED)
# find_package(OpenCV REQUIRED PATHS "")
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

#Mosquitto
find_library(Mosquitto_LIBS mosquitto)
if(NOT Mosquitto_LIBS)
    message(FATAL_ERROR "mosquitto kutuphanesi bulunamadi.")
    return()
endif()
message("mosquitto kutuphanesi bulundu.")
message("mosquitto kutuphanesi dizini: ${Mosquitto_LIBS}")

#Build
add_executable(montaj_v362_parca_kontrol_sistemi ./src/montaj_v362_parca_kontrol_sistemi.cpp)

#Link
target_link_libraries(montaj_v362_parca_kontrol_sistemi ${Mosquitto_LIBS} ${OpenCV_LIBS})
