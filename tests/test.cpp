#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

std::string readFileContents(const std::string &filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string normalize(const std::string &input) {
    std::string output = input;
    size_t pos = 0;
    // Replace all '\r\n' with '\n'
    while ((pos = output.find("\r\n", pos)) != std::string::npos) {
        output.replace(pos, 2, "\n");
    }
    return output;
}

TEST(FileComparisonTest, CompareOutputWithExpected) {
    std::string inputFilePath = INPUT_FILE_PATH;
    std::string outputFilePath = OUTPUT_FILE_PATH;
    std::string expectedFilePath = EXPECTED_FILE_PATH;
    std::string mainFilePath = EXECUTABLE_FILE_PATH;

    std::string command = mainFilePath + " --input " + inputFilePath +
                          " --output " + outputFilePath;
    system(command.c_str());

    std::string actualContents = normalize(readFileContents(outputFilePath));
    std::string expectedContents =
        normalize(readFileContents(expectedFilePath));

    ASSERT_EQ(actualContents, expectedContents)
        << "Output content differs from expected content!";
}
