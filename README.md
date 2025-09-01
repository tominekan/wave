# Wave

This is a quick tool to edit audio file metadata. I'm making this to avoid needing to go through Apple Music to edit the metadata of audio files that I add to my collection of files.


## Install
I'm not too sure how I'd be able to make an install script for Windows systems, so I only know how to make this work for MacOS and Linux systems.
This project does require TagLib as a dependency, and instructions for installing TagLib can be found [here](https://github.com/taglib/taglib/blob/master/INSTALL.md). 
Also, this project needs `C++17` or greater to be available in your system, otherwise it's won't compile because we use `std::filesystem`.

```sh
# Download this repo
git clone https://github.com/tominekan/wave

# Go to this directory
cd wave

# Make these executable
chmod +x install.sh

# Build and install wave (to /usr/local/bin)
./install.sh
```

**NOTE**: To install wave in a different path, go to install.sh and modify the `INSTALL_PATH` variable. If you get a permission denied error, then you might want to run `sudo ./install.sh`

### Wave TODOs:
- [x] Build a basic command line parser
- [x] Have it support editing important audio file metadata:
    - [x] Artist Names
    - [x] Album Title
    - [x] Album Cover
    - [x] Year
    - [x] Genre
- [ ] Maybe in the future have it also be capable of trimming audio files.
- [x] Develop argument parser support for shorter flags (`-v` versus `--version`)
- [x] Support operations for multiple files
    - [ ] Glob support
- [ ] Loudness normalization????????????????????

I think for legal reasons, I need to mention that I use [TagLib](https://taglib.org/api/index.html), and that TagLib is distributed under LGPL and MPL licenses.
