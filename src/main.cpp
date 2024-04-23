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
    std::cout << "Usage: project1 [options] [...]\n"
              << "Options:\n"
              << "  -h, --help\t\tDisplay help\n"
              << "  -f, --filepath\tSpecify the file path\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "error: no operation specified (use -h for help)\n";
        return 1;
    }

    int opt;
    std::string filepath;

    // Define the long options
    static struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"filepath", required_argument, nullptr, 'f'},
        {nullptr, 0, nullptr, 0}};
    // Parse the options
    while ((opt = getopt_long(argc, argv, "hf:", long_options, nullptr)) !=
           -1) {
        switch (opt) {
        case 'h':
            displayHelp();
            return 0;
        case 'f':
            filepath = optarg;
            break;
        default:
            displayHelp();
            return 1;
        }
    }

    std::cout << readFile(filepath) << std::endl;

    return 0;
}