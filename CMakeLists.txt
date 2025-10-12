cmake_minimum_required(VERSION 3.16)
project(JogoTriatlo)

set(CMAKE_CXX_STANDARD 11)

find_package(raylib REQUIRED)

# Coleta todos os .cpp em src/ pro executável
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)