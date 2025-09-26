cmake_minimum_required(VERSION 3.14)

project(software-renderer)
add_compile_options(-Wall -Wextra $<$<CONFIG:Release>:-O3>)

find_package(SDL3 REQUIRED)

include_directories(${SDL3_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(third_party/cglm/ EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES} m cglm_headers)
