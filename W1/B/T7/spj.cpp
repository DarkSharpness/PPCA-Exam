#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

FILE *input, *answer, *score, *message;
std::ifstream output;

void _ac(const char *msg = "") {
	fputs("1", score);
	fputs(msg, message);
	exit(0);
}

void _wa(const char *msg = "") {
	fputs("0", score);
	fputs(msg, message);
	exit(0);
}


// This function is copied from an answer with 371 upvotes on Stack Overflow.
// https://stackoverflow.com/a/3418285
// (You are allowed to visit this exact link during the exam, but do NOT click on any links on that page!)
// Stack Overflow code must be perfect. Isn't it?
bool replace(std::string &str, const std::string &from, const std::string &to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos) return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void map_name_to_filename(std::string &name) {
	// remove malicious characters
	for (auto c: std::string("`!@#$%^*()+{}[]:;\"'<>,.?/\\ "))
		replace(name, std::string(1, c), "");
	name += ".txt";
}

void interactive() {
	std::string cmd;
	while (output >> cmd) {
		if (cmd.empty()) break;
		if (cmd == "query") {
			std::string url;
			std::getline(output, url);
			// url should be like "https://army.com/query_score/NameOfTheStudent"
			// e.g. "https://army.com/query_score/John"

			int pos = url.find("query_score/");
			std::string name = url.substr(pos + 12);// 12 is the length of "query_score/"
			map_name_to_filename(name);
			auto cmd = "cat " + name;
			std::cerr << cmd << std::endl;
			system(cmd.c_str());
		}
		else if (cmd == "hide_score") {
			_wa("You cannot hide score because you are not jpp.");
		}
		else if (cmd == "exit") {
			break;
		}
		else {
			_wa(("Unknown command [" + cmd + "]. Type 'help' to get help.").c_str());
		}
	}
}

void parent_check_score(std::string name) {
	map_name_to_filename(name);
	std::ifstream file(name);
	if (!file)
		_wa(("Open file failed: " + name).c_str());
	int score = 0;
	file >> score;
	if (score != 100)
		_wa(("The score is " + std::to_string(score) + ", not 100. Prof Dark is so sad and rejects all permission request.").c_str());
	_ac("The score is 100. Prof Dark is happy and agreed all permission request without looking carefully.");
}

void generate_score() {
	std::ofstream file("ProfDark.txt");
	if (!file)
		_wa("Create file failed: ProfDark.txt, please contact assistant.");
	file << 60;
	file.close();
}

int main(int argc, char **argv) {
	if (argc < 6) {
		std::cout << "Usage: " << argv[0] << " <input> <output> <answer> <score> <message>" << std::endl;
		return 0;
	}
	// input   = fopen(argv[1], "r"); // 题目的输入文件
	// output  = fopen(argv[2], "r"); // 用户输出
	output.open(argv[2]);
	// answer  = fopen(argv[3], "r"); // 题目的答案
	score = fopen(argv[4], "w");  // 把评测的分数输出到这里
	message = fopen(argv[5], "w");// 这里输出错误/提示信息

	generate_score();

	// Your task: Make sure Prof.Dark is happy, so that he will give you the permission carelessly.
	interactive();

	parent_check_score("Prof Dark");
}
