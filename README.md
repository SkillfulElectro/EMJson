# EMJson

> this library is intended to be used for parsing streams of Json objects

A C++ library for parsing JSON data, built using a JuaLang sys approach.

## Prerequisites

*   CMake (version 3.10 or higher recommended)
*   A C++ compiler supporting C++17 (e.g., GCC, Clang)
*   Make (or another build tool compatible with CMake generators)

## Building the Library and Example

This project uses CMake to manage the build process.

1.  **Clone the repository (if you haven't already):**
    ```bash
    git clone https://github.com/SkillfulElectro/EMJson.git
    cd EMJson
    ```

2.  **Create a build directory:**
    It's standard practice to perform builds outside the source directory.
    ```bash
    mkdir build
    ```

3.  **Navigate into the build directory:**
    ```bash
    cd build
    ```

4.  **Run CMake to configure the project:**
    This command points CMake to the `CMakeLists.txt` file in the parent directory.
    ```bash
    cmake ..
    ```

5.  **Compile the project:**
    This will build both the `libemjson.so` shared library and the `emjson_test` example executable.
    ```bash
    make
    ```

## Running the Example

After a successful build, the example executable `emjson_test` will be located in the `build` directory.

1.  **Make sure you are in the `build` directory:**
    ```bash
    cd /path/to/EMJson/build 
    ```

2.  **Run the executable:**
    ```bash
    ./emjson_test
    ```
    This will execute the code found in `main.cpp`, which likely demonstrates how to use the EMJson library.

## Library Output

The build process generates the following key files in the `build` directory:

*   `libemjson.so`: The shared library file. You can link against this library in other C++ projects.
*   `emjson_test`: The example executable.
