#include <iostream>
#include <unordered_map>

/**
 * Takes in comand line arguments, and returns an unordered map containing
 * flags and inputs, and outputs htme
 */
class ArgParser {
    public:
        std::unordered_map<std::string, std::string> results;

        /**
         * 
         */
        ArgParser(int argc, char *argv[], std::unordered_map<std::string, std::string> inputs) {
            for (int i = 1; i < argc; i++) {
                std::cout << argv[i] << "\n";
            }
        }
}

Arg