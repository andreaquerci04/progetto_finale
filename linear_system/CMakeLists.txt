# In un file CMakeLists.txt si deve innanzitutto specificare la versione
# minima di CMake necessaria per compilare il progetto. Nel nostro caso
# specifichiamo 3.20
cmake_minimum_required(VERSION 3.20)

project(sistema_lineare) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

find_package(Eigen3 REQUIRED)

add_executable(test_matrici test_matrici.cpp)
add_executable(system system.cpp)

target_link_libraries(test_matrici PRIVATE Eigen3::Eigen)
target_link_libraries(system PRIVATE Eigen3::Eigen)