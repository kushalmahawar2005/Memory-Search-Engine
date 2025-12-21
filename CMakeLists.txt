cmake_minimum_required(VERSION 3.14)
project(search_engine)

# 1. Find Python (headers + libs)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# 2. Ask Python where pybind11 headers are
execute_process(
    COMMAND "${Python_EXECUTABLE}" -c
            "import pybind11; print(pybind11.get_include())"
    OUTPUT_VARIABLE PYBIND11_INCLUDE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 3. Build module
add_library(search_engine MODULE search_engine.cpp)

# 4. Include directories
target_include_directories(search_engine PRIVATE
    ${Python_INCLUDE_DIRS}
    ${PYBIND11_INCLUDE}
)

# 5. Link Python library
target_link_libraries(search_engine PRIVATE
    ${Python_LIBRARIES}
)

# 6. Windows Python extension settings
set_target_properties(search_engine PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
    CXX_STANDARD 17
)
