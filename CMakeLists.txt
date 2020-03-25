# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Define project name
project(montaj_v362_parca_kontrol_sistemi)
file(GLOB montaj_v362_parca_kontrol_sistemi *.hpp *.cpp)

# Find OpenCV
find_package(OpenCV 4 REQUIRED)


#Mosquitto
find_library(Mosquitto_LIBS mosquitto)
if(NOT Mosquitto_LIBS)
    message(FATAL_ERROR "mosquitto kutuphanesi bulunamadi.")
    return()
endif()
message("mosquitto kutuphanesi bulundu.")
message("mosquitto kutuphanesi dizini: ${Mosquitto_LIBS}")

#Build
add_executable(montaj_v362_parca_kontrol_sistemi ./src/montaj_v362_parca_kontrol_sistemi.cpp ./src/kamera_baglanti_kur.cpp)

#Link
target_link_libraries(montaj_v362_parca_kontrol_sistemi ${Mosquitto_LIBS} ${OpenCV_LIBS})
