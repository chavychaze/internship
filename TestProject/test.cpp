#include "pch.h"

class ThreadsTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any resources before each test
        GlobalData::getInstance().global_stats = GlobalStats();
    }

    void TearDown() override {
        // Clean up any resources after each test
    }
};

TEST_F(ThreadsTests, TrimFunction) {
    EXPECT_EQ(Threads::trim("  hello  "), "hello");
    EXPECT_EQ(Threads::trim("   "), "");
    EXPECT_EQ(Threads::trim("  \t\n  "), "");
    EXPECT_EQ(Threads::trim("test"), "test");
}

TEST_F(ThreadsTests, AnalyzeFileFunction) {
    // Create a temporary file with known content for testing
    std::ofstream testFile("test_file.txt");
    testFile << "/* Comment */\n";
    testFile << "  \n";
    testFile << "int main() {\n";
    testFile << "    std::cout << \"Hello, World!\";\n";
    testFile << "    return 0;\n";
    testFile << "}\n";
    testFile.close();

    // Analyze the temporary file
    fs::path filePath("test_file.txt");
    FileStats stats = Threads::analyze_file(filePath);

    EXPECT_EQ(stats.blank_lines, 1);
    EXPECT_EQ(stats.comment_lines, 1);
    EXPECT_EQ(stats.code_lines, 4);

    // Clean up the temporary file after the test
    fs::remove(filePath);
}

// Add more test cases for other functions in Threads.cpp

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
