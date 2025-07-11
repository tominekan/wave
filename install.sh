#!/bin/bash

# If any commands fail, the whole thing fails lol
set -euo pipefail

# Build wave, we're assuming taglib is installed 
g++ -std=c++17 wave.cpp -o wave -ltag -lz

# Install taglib to a specified directory
if [-f "wave" ]; then 
    echo "Moving binary to installation directory"
    mv wave $INSTALL_PATH
    echo "Successfully installed wave..."
else 
    echo "wave binary doesn't exist, did you run build.sh?"
fi

