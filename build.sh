#!/bin/bash
# Build script for Streamlit Cloud
cd /mount/build
cmake ..
make
cp search_engine*.so ../
