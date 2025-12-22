cmake_minimum_required(VERSION 3.15)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(pybind11 REQUIRED)

pybind11_add_module(search_engine ../search_engine.cpp)
