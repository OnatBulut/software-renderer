cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
    cglm_external
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG        v0.9.6
)

FetchContent_GetProperties(cglm_external)
if(NOT cglm_external_POPULATED)
    FetchContent_MakeAvailable(cglm_external)
endif()
