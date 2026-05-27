# In un file CMakeLists.txt si deve innanzitutto specificare la versione
# minima di CMake necessaria per compilare il progetto. Nel nostro caso
# specifichiamo 3.20
cmake_minimum_required(VERSION 3.20)

project(esercitazione8) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

#per trovare eigen3 ho chiesto l'aiuto a Gemini
find_package(Eigen3 REQUIRED)

add_executable(gradiente_coniugato gradiente_coniugato.cpp)

target_include_directories(gradiente_coniugato PRIVATE ${EIGEN3_INCLUDE_DIRS})