###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


###
# includes
###
include_directories(${PROJECT_SOURCE_DIR}/includes
                    ${CPP_REDIS_INCLUDES})


###
# executable
###
add_executable(giredis giredis.cpp)
target_link_libraries(giredis cpp_redis)
set_target_properties(giredis
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")