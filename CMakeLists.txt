###
# config
###
cmake_minimum_required(VERSION 2.8.1)
cmake_policy(SET CMP0042 NEW)

project(geospatical CXX)

###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CPP_REDIS_INCLUDES ${PROJECT_SOURCE_DIR}/includes/cpp_redis)

###
# includes
###

add_subdirectory(${CPP_REDIS_INCLUDES})

include_directories(${CPP_REDIS_INCLUDES}/includes ${CPP_REDIS_INCLUDES})

###
# executable
###
add_executable(giredis giredis.cpp)
target_link_libraries(giredis cpp_redis)
set_target_properties(giredis
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")