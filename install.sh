#!/bin/bash

# If any commands fail, the whole thing fails lol
set -euo pipefail
INSTALL_PATH=/usr/local/bin

# -ltag includes the taglib library, and lz includes the zlib library
if ! taglib-config --version then
    echo "Couldn't find taglib in your system, do you have it installed?"
    exit 1
fi

# Build taglib
g++ -std=c++17 wave.cpp -o wave -ltag -lz

# Install taglib to a specified directory
if [-f "wave" ]; then 
    echo "Moving binary to installation directory"
    mv wave $INSTALL_PATH
    echo "Successfully installed wave..."
else 
    echo "wave binary doesn't exist, did you run build.sh?"
fi

