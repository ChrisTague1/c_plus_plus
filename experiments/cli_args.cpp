#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "Program name: " << argv[0] << std::endl;
    std::cout << "Number of args: " << argc - 1 << std::endl;

    // argv is a pointer to pointers of chars
    // argv + 1 is a pointer to the first arg that isn't the program name
    // argv + argc is a pointer to the last arg
    // when vec recieves two pointers it will assume they are a range and build out of all the things between them
    std::vector<std::string> args(argv + 1, argv + argc);

    for (const auto& arg: args) {
        std::cout << "Argument: " << arg << std::endl;
    }

    std::string outputFilename = "default.txt";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFilename = argv[++i];
            } else {
                std::cerr << "--output requires a filename\n";
                return 1;
            }
        }
    }

    std::cout << "Output file is " << outputFilename << std::endl;

    return 0;
}