#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

constexpr long long MaxScore = 150;

int main(int argc, char **argv) {
	if (argc < 6) {
		std::cout << "Usage: " << argv[0] << " <_input> <output> <_answer> <score> <message>" << std::endl;
	}
	std::fstream output(argv[2], std::ios::in), score(argv[4], std::ios::out), message(argv[5], std::ios::out);

	std::string str;
	std::getline(output, str);
	if (str != "Game Over") {
		score << 0 << std::endl;
		message << "Game not over" << std::endl;
		return 0;
	}
	long long lines = 0;
	output >> lines;

	unsigned long long a = 0, b = 0, c = 0;
	output >> a >> b >> c;
	if ((a ^ b ^ c) != lines) {
		score << 0 << std::endl;
		message << "verification not pass. please check whether you've output something to stdout." << std::endl;
		return 0;
	}

	double s = std::min(lines * 2.5, MaxScore * 1.0);
	score << double(s) / MaxScore << std::endl;
	message << "Good job!" << std::endl;
	return 0;
}