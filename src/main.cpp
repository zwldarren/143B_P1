#include "Manager.h"
#include <fstream>
#include <getopt.h>
#include <iostream>

std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Function to read a file
std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    }

    Manager manager = Manager();
    std::string line, output;
    bool shouldReset = false; // Flag to check if we need to reset the manager

    while (std::getline(file, line)) {
        line = trim(line);
        std::istringstream stream(line);
        std::string cmd;
        stream >> cmd;

        // Check if the command is 'in' or 'id', if so, reset the manager
        if (cmd == "in" || cmd == "id") {
            if (shouldReset) {
                manager = Manager();
                output = output.substr(0, output.size() - 1);
                output += "\n";
            }
            shouldReset = true;
        }

        if (!line.empty()) {
            int commandResult = manager.executeCommand(line);
            output += std::to_string(commandResult) + " ";
        }
    }

    file.close();
    return output;
}

void displayHelp() {
    std::cout << "Usage: [options]\n"
              << "Options:\n"
              << "  -h, --help          Show this help message\n"
              << "  -i, --input <file>  Specify the input file\n"
              << "  -o, --output <file> Specify the output file\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "error: no operation specified (use -h for help)\n";
        return 1;
    }

    int opt;
    std::string inputPath;
    std::string outputPath;

    // Define the long options
    static struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {nullptr, 0, nullptr, 0}};
    // Parse the options
    while ((opt = getopt_long(argc, argv, "hi:o:", long_options, nullptr)) !=
           -1) {
        switch (opt) {
        case 'h':
            displayHelp();
            return 0;
        case 'i':
            inputPath = optarg;
            break;
        case 'o':
            outputPath = optarg;
            break;
        default:
            displayHelp();
            return 1;
        }
    }

    std::string content = readFile(inputPath);

    if (!outputPath.empty()) {
        std::ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
            std::cerr << "error: could not open output file '" << outputPath
                      << "'\n";
            return 1;
        }
        outputFile << content;
        outputFile.close();
    } else {
        std::cout << content << std::endl;
    }

    return 0;
}