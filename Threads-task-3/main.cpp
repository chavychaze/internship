#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <filesystem>

// Use std::filesystem for directory traversal
namespace fs = std::filesystem;

std::mutex results_mutex;

struct FileStats {
	int blank_lines = 0;
	int comment_lines = 0;
	int code_lines = 0;
};

struct GlobalStats {
	int total_files = 0;
	int total_blank_lines = 0;
	int total_comment_lines = 0;
	int total_code_lines = 0;
	void merge(const FileStats& fs) {
		total_blank_lines += fs.blank_lines;
		total_comment_lines += fs.comment_lines;
		total_code_lines += fs.code_lines;
		total_files++;
	}
} global_stats;

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}

// Analyze a single file and return the stats
FileStats analyze_file(const fs::path& file_path) {
	FileStats stats;
	std::ifstream file(file_path);
	std::string line;

	while (std::getline(file, line)) {
		// Strip whitespace and check for blank or comment lines
		// This is a simplified check; you might need more complex logic here
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

// The worker thread function
void process_files(const std::vector<fs::path>& files) {
	for (const auto& file_path : files) {
		FileStats stats = analyze_file(file_path);
		{
			std::lock_guard<std::mutex> guard(results_mutex);
			global_stats.merge(stats);
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <root_folder>" << std::endl;
		return 1; // Return an error code to indicate incorrect usage
	}

	// Parse command-line arguments or configuration to get the root folder
	std::string root_folder = argv[1];

	auto start_time = std::chrono::high_resolution_clock::now();

	// Traverse directory and collect all files to process
	std::vector<fs::path> all_files;
	for (const auto& entry : fs::recursive_directory_iterator(root_folder)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp") {
				all_files.push_back(entry.path());
			}
		}
	}

	// Split file paths between threads
	unsigned int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	std::vector<std::vector<fs::path>> files_per_thread(num_threads);

	// Simple load balancing by assigning files to threads in a round-robin fashion
	for (size_t i = 0; i < all_files.size(); ++i) {
		files_per_thread[i % num_threads].push_back(all_files[i]);
	}

	// Launch threads
	for (auto& file_batch : files_per_thread) {
		threads.emplace_back(process_files, file_batch);
	}

	// Join threads
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end_time - start_time;

	// Output the results
	std::cout << "Total number of processed files: " << global_stats.total_files << std::endl;
	std::cout << "Total number of blank lines: " << global_stats.total_blank_lines << std::endl;
	std::cout << "Total number of comment lines: " << global_stats.total_comment_lines << std::endl;
	std::cout << "Total number of code lines: " << global_stats.total_code_lines << std::endl;
	std::cout << "Time of execution: " << diff.count() << " seconds" << std::endl;

	// Save the results to a file
	std::ofstream outfile("results.txt");
	outfile << "Total number of processed files: " << global_stats.total_files << "\n";
	outfile << "Total number of blank lines: " << global_stats.total_blank_lines << "\n";
	outfile << "Total number of comment lines: " << global_stats.total_comment_lines << "\n";
	outfile << "Total number of code lines: " << global_stats.total_code_lines << "\n";
	outfile << "Time of execution: " << diff.count() << " seconds" << "\n";
	outfile.close();

	return 0;
}