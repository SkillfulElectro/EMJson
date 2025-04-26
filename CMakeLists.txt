# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(EMJson LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── OUTPUT DIRECTORIES ────────────────────────────────────────────────
if (CMAKE_CONFIGURATION_TYPES)  # multi-config (Visual Studio, Xcode)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR})
else()                           # single-config (Makefiles, Ninja)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR})
endif()

# ── INCLUDES ──────────────────────────────────────────────────────────
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/sys
)

# ── LIBRARY ───────────────────────────────────────────────────────────
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

# ── TEST EXECUTABLE ───────────────────────────────────────────────────
add_executable(emjson_test main.cpp)
target_link_libraries(emjson_test PUBLIC emjson)

# ── TESTING ───────────────────────────────────────────────────────────
enable_testing()
add_test(NAME RunEmjsonTest COMMAND emjson_test)
