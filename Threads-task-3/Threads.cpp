#include "Threads.hpp"

void Threads::process_files(const std::vector<fs::path>& files) {
	for (const auto& file_path : files) {
		FileStats stats = analyze_file(file_path);
		{
			std::lock_guard<std::mutex> guard(GlobalData::getInstance().results_mutex);
			GlobalData::getInstance().global_stats.merge(stats);
		}
	}
}

std::string Threads::trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}

FileStats Threads::analyze_file(const fs::path& file_path) {
	FileStats stats;
	std::ifstream file(file_path);
	std::string line;

	while (std::getline(file, line)) {
		std::string trimmed = trim(line);
		if (trimmed.empty()) {
			stats.blank_lines++;
		}
		else if (trimmed.starts_with("//") || trimmed.starts_with("/*") || trimmed.starts_with("*/")) {
			stats.comment_lines++;
		}
		else {
			stats.code_lines++;
		}
	}

	return stats;
}
