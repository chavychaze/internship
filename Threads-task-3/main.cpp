#include "Threads.hpp"
#include "GlobalData.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <root_folder>" << std::endl;
		return 1; // Return an error code to indicate incorrect usage
	}

	std::string root_folder = argv[1];

	auto start_time = std::chrono::high_resolution_clock::now();

	std::vector<fs::path> all_files;
	for (const auto& entry : fs::recursive_directory_iterator(root_folder)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp") {
				all_files.push_back(entry.path());
			}
		}
	}

	unsigned int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	std::vector<std::vector<fs::path>> files_per_thread(num_threads);

	for (size_t i = 0; i < all_files.size(); ++i) {
		files_per_thread[i % num_threads].push_back(all_files[i]);
	}

	for (auto& file_batch : files_per_thread) {
		threads.emplace_back(Threads::process_files, file_batch);
	}

	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end_time - start_time;

	std::cout << "Total number of processed files: " << GlobalData::getInstance().global_stats.total_files << std::endl;
	std::cout << "Total number of blank lines: " << GlobalData::getInstance().global_stats.total_blank_lines << std::endl;
	std::cout << "Total number of comment lines: " << GlobalData::getInstance().global_stats.total_comment_lines << std::endl;
	std::cout << "Total number of code lines: " << GlobalData::getInstance().global_stats.total_code_lines << std::endl;
	std::cout << "Time of execution: " << diff.count() << " seconds" << std::endl;

	std::ofstream outfile("results.txt");
	outfile << "Total number of processed files: " << GlobalData::getInstance().global_stats.total_files << "\n";
	outfile << "Total number of blank lines: " << GlobalData::getInstance().global_stats.total_blank_lines << "\n";
	outfile << "Total number of comment lines: " << GlobalData::getInstance().global_stats.total_comment_lines << "\n";
	outfile << "Total number of code lines: " << GlobalData::getInstance().global_stats.total_code_lines << "\n";
	outfile << "Time of execution: " << diff.count() << " seconds" << "\n";
	outfile.close();

	return 0;
}
