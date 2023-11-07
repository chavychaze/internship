#pragma once

#include <filesystem>
#include <vector>
#include <thread>
#include <fstream>

#include "GlobalData.hpp"

namespace fs = std::filesystem;

class Threads {
public:
	Threads() = delete; // This class should not be instantiated
	static void process_files(const std::vector<fs::path>& files);
	static std::string trim(const std::string& str);
	static FileStats analyze_file(const fs::path& file_path);
};
