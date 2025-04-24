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


target_compile_features(emjson PRIVATE cxx_std_17)


add_executable(emjson_test main.cpp)


target_link_libraries(emjson_test PRIVATE emjson)

