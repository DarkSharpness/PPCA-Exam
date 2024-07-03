#include "game.h"
#include <algorithm>
#include <fstream>
#include <iostream>

constexpr long long MaxScore = 150;

int main(int argc, char **argv) {
	if (argc < 6) {
		std::cout << "Usage: " << argv[0] << " <_input> <output> <_answer> <score> <message>" << std::endl;
		return 0;
	}
	std::fstream output(argv[2], std::ios::in), score(argv[4], std::ios::out), message(argv[5], std::ios::out);
	long long lines = 0;
	try {
		int n = 0;
		output >> n;
		int shape_id, col, rotation_deg;
		for (int i = 0; i < n; ++i) {
			output >> shape_id >> col >> rotation_deg;
			game.step(shape_id, col, rotation_deg);
		}
		game.game_over();
	} catch (unsigned int score) {
		lines = score;
	} catch (...) {
		score << 0 << std::endl;
		message << "Exception thrown, please contact TA." << std::endl;
		return 0;
	}
	double s = std::min(lines * 2.5, MaxScore * 1.0);
	score << double(s) / MaxScore << std::endl;
	message << "Good job!" << std::endl;
	return 0;
}
