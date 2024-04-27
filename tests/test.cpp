#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

std::string readFileContents(const std::string &filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST(FileComparisonTest, CompareFiles) {
    std::string outputFilePath = "${CMAKE_CURRENT_BINARY_DIR}/test_output.txt";
    std::string expectedFilePath =
        "${CMAKE_CURRENT_SOURCE_DIR}/data/sample-output.txt";

    std::string actualContents = readFileContents(outputFilePath);
    std::string expectedContents = readFileContents(expectedFilePath);

    ASSERT_EQ(actualContents, expectedContents)
        << "Files " << outputFilePath << " and " << expectedFilePath
        << " differ!";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}