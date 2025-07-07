# Wave

This is a quick tool to edit audio file metadata. I'm making this to avoid needing to go through Apple Music to edit the metadata of audio files that I add to my collection of files.


## Install
I'm not too sure how I'd be able to make an install script for Windows systems, so I only know how to make this work for MacOS and Linux systems.

```sh
# Install TagLib

# Make these executable
chmod +x build.sh install.sh

# Build the project
./build.sh

# Install wave (to /usr/local/bin)
./install.sh
```

### Wave TODOs:
- [x] Build a basic command line parser
- [ ] Have it support editing important audio file metadata:
    - [ ] Artists
    - [ ] Album Title
    - [ ] Album Cover
    - [ ] Year
    - [ ] Genre
- [ ] Maybe in the future have it also be capable of trimming audio files.
- [ ] Have command line parser support shorter flags (`-v` versus `--version`)

I think for legal reasons, I need to me mention that I use [TagLib](https://taglib.org/api/index.html), and that TagLib is distributed under LGPL and MPL licenses.