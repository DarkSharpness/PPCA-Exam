#include <fstream>
#include <string>
#include <iostream>

unsigned int simpleHash(const std::string &input) {
	unsigned int hash = 0;
	for (char c: input) {
		hash = ((hash * 31) + c) % 100000007;
	}
	return hash;
}

bool check(const std::string &input) {
	for (auto c : input) {
		if (!std::isalnum(c) && c != ' ' && c != '.')
			return false;
	}
	return true;
}

int main(int argc, char *argv[]) {
	// std::cout << std::hex << simpleHash("DarkSharpness is watching you.");
	// > 0x46ee11e
	std::fstream output(argv[2], std::ios::in), score(argv[4], std::ios::out), message(argv[5], std::ios::out);
	std::string s;
	output >> s;
	if (!check(s)) {
		score << 0;
		message << "invalid description";
		return 0;
	}
	if (simpleHash(s) == 0x46ee11e) {
		score << 1;
		message << "correct description";
	}
	else {
		score << 0;
		message << "wrong description";
	}
	return 0;
}