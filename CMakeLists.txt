cmake_minimum_required (VERSION 2.6)
project(Ptex2Uv)

# Set up include and lib paths
set(EXTERNAL "ThirdParty")
include_directories("${EXTERNAL}/tinyobjloader")

add_subdirectory(src)

add_executable(${CMAKE_PROJECT_NAME}
    "src/main.cpp"
    )
    