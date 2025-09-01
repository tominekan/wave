/**
 * This is a script for the basic audio manipulations I find myself constantly doing. 
 * Specifically, getting metadata. I'm trying to make this as useful for me as possible. 
 */
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>

/**
 * Returns an unordered map representing command line arguments.
 * `inputs` is a hash map formatted like so: Keys are the names of the command line flags,
 * and the value of the keys represents the type of the flag associated with the name.
 * There are two types we can specify, `bool` for boolean flags, and `str` (really anything not bool), for all other flags.
 * Boolean flags are true if they exist within argv, and `str` flags require a value right after the flag. 
 * 
 * For all values that are non-flags, we return them in a vector keyed to ""  (an empty string).
 * If we have an unrecognized flag, we throw an error. 
 * 
 * @param argc the number of command line arguments
 * @param argv the command in list form
 * @param inputs the formatted input
 * @returns a dictionary representing the command line arguments as specified above
 */
std::unordered_map<std::string, std::string> parseArguments(int argc, char *argv[], std::unordered_map<std::string, std::string> inputs) {
    std::unordered_map<std::string, std::string> result;
    std::unordered_map<std::string, std::string> flags;
    std::string non_flags = "";

    for (int i = 1; i < argc; i++) {
        std::string current = argv[i];
        size_t str_size = current.size();

        // This means this some sort of command line flag
        if ((str_size >= 2) && (current.compare(0, 1, "-") == 0)) {
            try {
                // Long command line flag
                if (current.compare(0, 2, "--") == 0) {
                    std::string flag_name = current.substr(2, str_size - 2);
                    std::string type = inputs.at(flag_name);

                    if (type == "bool") {
                        result.insert({flag_name, "true"});
                    } else {
                        if (i != (argc - 1)) {
                            std::string flag_value = argv[++i];
                            result.insert({flag_name, flag_value});
                        } else {
                            // Flag is last in set of arguments
                            throw std::invalid_argument("");
                        }
                    }
                } else {
                    std::string flag_name = current.substr(1, str_size - 1);
                    std::string type = inputs.at(flag_name);

                    if (type == "bool") {
                        result.insert({flag_name, "true"});
                    } else {
                        if (i != (argc - 1)) {
                            std::string flag_value = argv[++i];
                            result.insert({flag_name, flag_value});
                        } else {
                            // Flag is last in set of arguments
                            throw std::invalid_argument("");
                        }
                    }
                }

            } catch (const std::out_of_range& e) {
                std::cerr << "Flag \"" << current << "\" unrecognized\n";
                exit(-1);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Flag \"" << current << "\" needs an associated value\n";
                exit(-1);
            }

        } else {
            // It's not a flag, so we add it to non-flags
            non_flags += current + " ";
        }
    }

    result.insert({"", non_flags});
    return result;
}

/**
 * Splits a string into a vector of items based on a delimiter.
 * 
 * @param text the string we want to split up
 * @param delim the delimiter we are splitting the string by
 * @returns vector representing the string split up by `delim`
 */
std::vector<std::string> split_string(std::string text, char delim) {
    std::vector<std::string> result;
    std::string elem = "";
    for (char c : text) {
        if (c == delim) {
            if (elem != "") {
                result.push_back(elem);
                elem = "";
            }
        } else {
            elem += c;
        }
    }
    return result;
}

/**
 * Checks if `key` exists in the specified unordered_map.
 * @param map the unordered_map we want to check if the key exists in
 * @param key the key that we want to see if exists
 * @returns true if the key exists, false otherwise
 */
bool containsKey(std::unordered_map<std::string, std::string> map, std::string key) {
    return (map.find(key) != map.end());
}

/**
 * Prints that we're changing a specific value
 * @param value the name of the value we're changing
 * @param oldv the old value
 * @param newv new value
 */
void printChange(std::string value, std::string oldv, std::string newv) {
    std::cout << "Setting " << value << " : \"" << oldv << "\" -> \"" << newv << "\" \n";
}

/**
 * Prints that we're changing a specific value
 * @param value the name of the value we're changing
 * @param oldv the old value
 * @param the new value
 */
void printChange(std::string value, TagLib::String oldv, std::string newv) {
    std::cout << "Setting " << value << " : \"" << oldv << "\" -> \"" << newv << "\" \n";
}


/**
 * Grabs an image file and turns it into a TagLib ByteVector.
 * @param path the path to the image file
 * @returns the byte vector representation of the file
 */
TagLib::ByteVector vectorFromFile(std::string path) {
    if (std::filesystem::is_directory(path)) {
        std::cerr << path << ": not a file";
        exit(-1);
    }
    std::ifstream file (path, std::ios::binary);
    if (!file) {
        std::cerr << path << ": not a file";
        exit(-1);
    }

    std::vector<char> buffer ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return TagLib::ByteVector (buffer.data(), buffer.size());
}


int main(int argc, char *argv[]) {
    try {
        // Set our expected arguments
        std::unordered_map<std::string, std::string> expectedArgs = {
            {"help", "bool"},
            {"h", "bool"},
            {"set-artist", "str"},
            {"a", "str"},
            {"set-title", "str"},
            {"t", "str"},
            {"set-album", "str"},
            {"l", "str"},
            {"set-genre", "str"},
            {"g", "str"},
            {"set-year", "str"},
            {"y", "str"},
            {"set-art", "str"},
            {"r", "str"},
            {"summarize", "bool"},
            {"s", "bool"},
            {"verbose", "bool"},
            {"vv", "bool"},
            {"version", "bool"},
            {"v", "bool"}
        };
        // Parse the command line arguments
        std::unordered_map<std::string, std::string> args = parseArguments(argc, argv, expectedArgs);

        // Split dat bih by spaces
        std::vector<std::string> music_files = split_string(args.at(""), ' ');

        bool verbose = containsKey(args, "verbose") || containsKey(args, "vv");

        if (containsKey(args, "help") || containsKey(args, "h")) {
            std::cout << "OVERVIEW: Wave v2.0. A command line tool to help with basic audio metadata editing.\n\n";
            std::cout << "USAGE: wave audio_file.mp3 [options]\n\n";
            std::cout << "OPTIONS:\n";
            std::cout << "--version    (-v)   Outputs wave's current version\n";
            std::cout << "--verbose    (-vv)  Prints out what wave is currently doing\n";
            std::cout << "--summarize  (-s)   Outputs a summary of the track.\n";
            std::cout << "--set-artist (-a)   Sets the artist(s) of the track.\n";
            std::cout << "--set-title  (-t)   Sets the title of the track.\n";
            std::cout << "--set-album  (-l)   Sets the album to which the track belongs to.\n";
            std::cout << "--set-genre  (-g)   Sets the genre(s) to which the track belongs to.\n";
            std::cout << "--set-year   (-y)   Sets the year in which the track was published.\n";
            std::cout << "--set-art    (-r)   Sets the cover of the track to a specfied image path, only supports jpeg and png.\n";
            std::cout << "--help       (-h)   Displays this help message and exits.\n";
            exit(0);
        }

        if (containsKey(args, "version") || containsKey(args, "v")) {
            std::cout << "Wave v2.0";
            exit(0);
        }

        // It's mandatory we supply an input file
        if (music_files.size() == 0) {
            std::cerr << "program needs input file.\n";
            exit(-1);
        }

        for (std::string file : music_files) {
            // Ensure each file exists
            if (!std::filesystem::is_regular_file(file)) {
                std::cerr << "file \"" << file << "\" exists"; 
                exit(-1);
            }

            TagLib::FileRef f(file.c_str());
            TagLib::Tag* tag = f.tag();
            if (containsKey(args, "summarize") || containsKey(args, "s")) {
                std::cout << "Title: " << tag->title() << "\n";
                std::cout << "Artist: " << tag->artist() << "\n";
                std::cout << "Album: " << tag->album() << "\n";
                std::cout << "Year: " << tag->year() << "\n";
                std::cout << "Genre: " << tag->genre() << "\n";
            }

            if (containsKey(args, "set-artist") || containsKey(args, "a")) {
                std::string newArtist = "";
                if (containsKey(args, "set-artist")) {
                    newArtist = args.at("set-artist");
                } else {
                    newArtist = args.at("a");
                }

                if (verbose) {
                    printChange("Artist", tag->artist(), newArtist);
                }
                tag->setArtist(newArtist);
            }

            if (containsKey(args, "set-title") || containsKey(args, "t")) {
                std::string newTitle = "";
                if (containsKey(args, "set-title")) {
                    newTitle = args.at("set-title");
                } else {
                    newTitle = args.at("t");
                }

                if (verbose) {
                    printChange("Title", tag->title(), newTitle);
                }
                tag->setTitle(newTitle);
            }

            if (containsKey(args, "set-album") || containsKey(args, "l")) {
                std::string newAlbum = "";
                if (containsKey(args, "set-album")) {
                    newAlbum = args.at("set-album");
                } else {
                    newAlbum = args.at("l");
                }

                if (verbose) {
                    printChange("Album", tag->album(), newAlbum);
                }
                tag->setAlbum(newAlbum);
            }

            if (containsKey(args, "set-year") || containsKey(args, "y")) {
                std::string newYear = "";
                if (containsKey(args, "set-year")) {
                    newYear = args.at("set-year");
                } else {
                    newYear = args.at("y");
                }

                if (verbose) {
                    printChange("Year", tag->album(), newYear);
                }
                tag->setYear(std::stoi(newYear));
            }

            if (containsKey(args, "set-genre") || containsKey(args, "g")) {
                std::string newGenre = "";
                if (containsKey(args, "set-genre")) {
                    newGenre = args.at("set-genre");
                } else {
                    newGenre = args.at("g");
                }

                if (verbose) {
                    printChange("Genre", tag->album(), newGenre);
                }
                tag->setGenre(newGenre);
            }

            if (containsKey(args, "set-art") || containsKey(args, "r")) {
                std::string imagePath = "";
                if (containsKey(args, "set-art")) {
                    imagePath = args.at("set-art");
                } else {
                    imagePath = args.at("r");
                }

                size_t length = imagePath.size();
                std::string jpeg = ".jpeg";
                std::string png = ".png";
                std::string jpg = ".jpg";
                std::string mime_type = "image/png";

                bool isJPEG = (imagePath.compare(length - 5, 5, jpeg) == 0);
                bool isPNG = (imagePath.compare(length - 4, 4, png) == 0);
                bool isJPG = (imagePath.compare(length - 3, 3, jpg) == 0);

                // If the file is NEITHER jpeg nor png
                if (!(isJPEG || isPNG || isJPG)) {
                    std::cerr << "Image file type is unsupported: must be jpeg (or jpg) or png\n";
                    exit(-1);
                }

                // Is the file jpeg (jpeg or jpg)
                if (isJPEG || isJPG) {
                    mime_type = "image/jpeg";
                }


                if (verbose) {
                    std::cout << "Setting album cover to: " << args.at("set-art") << "\n";
                }

                // Create variant map describing picture type
                TagLib::ByteVector data = vectorFromFile(imagePath);
                TagLib::VariantMap pic_map;
                pic_map["data"] = data;
                pic_map["pictureType"] = "Front Cover";
                pic_map["mimeType"] = mime_type.c_str();

                // Add to list of variant maps
                TagLib::List<TagLib::VariantMap> map_list;
                map_list.append(pic_map);

                // Set the album cover to our new cover
                f.setComplexProperties("PICTURE", map_list);
            }

            f.save();

        }
    } catch (...) {
        std::cerr << "\nSomething went wrong...\n";
        exit(1);
    }
    
    return 0;
}
