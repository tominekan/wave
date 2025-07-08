/**
 * This is a script for the basic audio manipulations I find myself constantly doing. 
 * Specifically, setting the audtho
 */
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <taglib/fileref.h>

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
    std::string flag_indicator = "--";

    for (int i = 1; i < argc; i++) {
        std::string current = argv[i];
        size_t str_size = current.size();

        // This means this is a command line flag
        if ((str_size > 2) && (current.compare(0, 2, flag_indicator) == 0)) {
            // Grab everything after "--" in the flag 
            std::string flag_name = current.substr(2, str_size - 2);

            // Should throw an error if the flag doesn't exist
            // Might want to handle this myself (will need to handle this by myself)
            try {
                std::string type = inputs.at(flag_name);
                if (type == "bool") {
                    result.insert({flag_name, "true"});
                } else {
                    // This is the value of the flags described
                    if (i != (argc - 1)) {
                        std::string flag_value = argv[++i];
                        result.insert({flag_name, flag_value});
                    } else {
                        throw std::invalid_argument("");
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
            // It's not a flag, so ew add it to non-flags
            non_flags += current + " ";
        }
    }

    result.insert({"", non_flags});
    return result;
}

/**
 * Splits a string into a vector of items based on a delmiiter.
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
 * @param the new value
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
int main(int argc, char *argv[]) {
    // Set our expected arguments
    std::unordered_map<std::string, std::string> expectedArgs = {
        {"help", "bool"},
        {"file", "str"},
        {"set-artist", "str"},
        {"set-title", "str"},
        {"set-album", "str"},
        {"summarize", "bool"},
        {"verbose", "bool"},
        {"version", "bool"}
    };

    // Parse the command line arguments
    std::unordered_map<std::string, std::string> args = parseArguments(argc, argv, expectedArgs);

    if (containsKey(args, "help")) {
        std::cout << "Wave. A command line tool to help with basic audio metadata editing.\n";
        exit(0);
    }

    if (containsKey(args, "version")) {
        std::cout << "Wave v0.1beta";
        exit(0);
    }

    // It's mandatory this this program exists
    if (!containsKey(args, "file")) {
        std::cerr << "program needs input file.\n";
        exit(-1);
    }
    
    // Ensure that the file exists
    if (!std::filesystem::is_regular_file(args.at("file"))) {
        std::cerr << "file \"" << args.at("file") << "\" exists"; 
        exit(-1);
    }

    // Now that we can guarantee that the file exists, we can move on
    try {
        TagLib::FileRef f(args.at("file").c_str());
        TagLib::Tag* tag = f.tag();
        if (containsKey(args, "summarize")) {
            std::cout << "Title: " << tag->title() << "\n";
            std::cout << "Artist: " << tag->artist() << "\n";
            std::cout << "Album: " << tag->album() << "\n";
            std::cout << "Year: " << tag->year() << "\n";
            std::cout << "Genre: " << tag->genre() << 
        }

        if (containsKey(args, "set-artist")) {
            if (containsKey(args, "verbose")) {
                printChange("Artist", tag->artist(), args.at("set-artist"));
            }
            tag->setArtist(args.at("set-artist"));
        }

        if (containsKey(args, "set-title")) {
            if (containsKey(args, "verbose")) {
                printChange("Title", tag->title(), args.at("set-title"));
            }
            tag->setTitle(args.at("set-title"));
        }

        if (containsKey(args, "set-album")) {
            if (containsKey(args, "verbose")) {
                printChange("Album", tag->album(), args.at("set-album"));
            }
            tag->setAlbum(args.at("set-album"));
        }

        if (containsKey(args, "set-year")) {
            if (containsKey(args, "verbose")) {
                printChange("Year", tag->album(), args.at("set-year"));
            }
            tag->setYear(args.at("set-year"));
        }

        if (containsKey(args, "set-genre")) {
            if (containsKey(args, "verbose")) {
                printChange("Genre", tag->album(), args.at("set-genre"));
            }
            tag->setGenre(args.at("set-genre"));
        }
    } catch (...) {
        std::cerr << "Something went wrong...";
        exit(1);
    }
    
    return 0;
}
