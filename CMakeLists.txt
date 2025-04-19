cmake_minimum_required(VERSION 3.10)
project(Akinator)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

if(APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-deprecated-declarations")
endif()