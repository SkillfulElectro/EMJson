cmake_minimum_required(VERSION 3.10)

project(EMJson LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(.)
include_directories(sys)


set(LIB_SOURCES
    em_json.cpp
    em_json_init.cpp
    em_json_actions.cpp
    em_jsondata.cpp
)


add_library(emjson SHARED ${LIB_SOURCES})

target_include_directories(emjson PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sys>
)

target_compile_features(emjson PRIVATE cxx_std_17)



set_target_properties(emjson PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" 
)



add_executable(emjson_test main.cpp)


set_target_properties(emjson_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" 
)



target_link_libraries(emjson_test PRIVATE emjson)


enable_testing()

add_test(
    NAME RunEmjsonTest
    COMMAND $<TARGET_FILE:emjson_test> 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin 
)
