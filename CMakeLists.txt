cmake_minimum_required(VERSION 3.22)
project(Nonograms C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(THIRD_PARTY ${CMAKE_CURRENT_LIST_DIR}/third_party)

find_package(raylib CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    ${SRC}/Grid.cpp
    ${SRC}/main.cpp 
    ${SRC}/Menu.cpp 
    ${SRC}/Solver.cpp 
    ${SRC}/Utils.cpp
    ${THIRD_PARTY}/raygui.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${THIRD_PARTY})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++exp)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic -march=native $<$<CONFIG:Release>:-flto> $<$<COMPILE_LANGUAGE:CXX>:-fconstexpr-ops-limit=1000000000>)

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
        target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address> $<$<CONFIG:Debug>:-static-libasan>)
    endif()
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
