#include "Manager.h"
#include <fstream>
#include <getopt.h>
#include <iostream>

// Function to read a file
std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    }

    Manager manager;
    std::string line;
    std::string output = "";
    while (std::getline(file, line)) {
        if (line == "\r" || line == "\n" || line == "\r\n") {
            manager = Manager();
            output += "\n";
        } else {
            output += std::to_string(manager.executeCommand(line)) + " ";
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