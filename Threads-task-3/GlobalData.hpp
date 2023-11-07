#pragma once

#include <mutex>

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
};

class GlobalData {
public:
	static GlobalData& getInstance() {
		static GlobalData instance;
		return instance;
	}

	GlobalStats global_stats;
	std::mutex results_mutex;

private:
	GlobalData() { }

	GlobalData(const GlobalData&) = delete;
	GlobalData& operator=(const GlobalData&) = delete;
};
