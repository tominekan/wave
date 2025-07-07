# -ltag includes the taglib library, and lz includes the zlib library
g++ -std=c++17 wave.cpp -o wave -ltag -lz
# -I/usr/local/include/taglib -L/usr/local/lib -v