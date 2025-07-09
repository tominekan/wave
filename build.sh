# -ltag includes the taglib library, and lz includes the zlib library
if ! taglib-config --version then
    echo "Couldn't find taglib in your system, do you have it installed?"
    exit 1
fi
g++ -std=c++17 wave.cpp -o wave -ltag -lz
