cmake_minimum_required(VERSION 3.13)
#Habilidamos el modulo de descarga de CMake
include(FetchContent)

#Obligar a google test a usar memoria dinamica (MDd)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# le decimos a CMake de donde sacar google test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG     release-1.12.1 #o la version mas reciente
)
#CMake descarga la libreria y la deja disponible
FetchContent_MakeAvailable(googletest)

#creamos ejecutable de pruebas
add_executable(test_abs test_abs.cpp)

#enlazamos tu ejecutable con la libreria gtest_main que acabamos de descargar
target_link_libraries(test_abs gtest_main)